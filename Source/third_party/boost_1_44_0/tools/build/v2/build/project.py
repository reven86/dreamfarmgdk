# Status: being ported by Vladimir Prus
# Base revision: 42507

# Copyright 2002, 2003 Dave Abrahams 
# Copyright 2002, 2005, 2006 Rene Rivera 
# Copyright 2002, 2003, 2004, 2005, 2006 Vladimir Prus 
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt) 

#  Implements project representation and loading.
#   Each project is represented by 
#   - a module where all the Jamfile content live. 
#   - an instance of 'project-attributes' class.
#     (given module name, can be obtained by 'attributes' rule)
#   - an instance of 'project-target' class (from targets.jam)
#     (given a module name, can be obtained by 'target' rule)
#
#  Typically, projects are created as result of loading Jamfile, which is
#  do by rules 'load' and 'initialize', below. First, module for Jamfile
#  is loaded and new project-attributes instance is created. Some rules
#  necessary for project are added to the module (see 'project-rules' module)
#  at the bottom of this file.
#  Default project attributes are set (inheriting attributes of parent project, if
#  it exists). After that, Jamfile is read. It can declare its own attributes,
#  via 'project' rule, which will be combined with already set attributes.
#
#
#  The 'project' rule can also declare project id, which will be associated with 
#  the project module.
#
#  There can also be 'standalone' projects. They are created by calling 'initialize'
#  on arbitrary module, and not specifying location. After the call, the module can
#  call 'project' rule, declare main target and behave as regular projects. However,
#  since it's not associated with any location, it's better declare only prebuilt 
#  targets.
#
#  The list of all loaded Jamfile is stored in variable .project-locations. It's possible
#  to obtain module name for a location using 'module-name' rule. The standalone projects
#  are not recorded, the only way to use them is by project id.

import b2.util.path
from b2.build import property_set, property
from b2.build.errors import ExceptionWithUserContext
import b2.build.targets

import bjam

import re
import sys
import os
import string
import imp
import traceback

class ProjectRegistry:

    def __init__(self, manager, global_build_dir):
        self.manager = manager
        self.global_build_dir = None
        self.project_rules_ = ProjectRules(self)

        # The target corresponding to the project being loaded now
        self.current_project = None
        
        # The set of names of loaded project modules
        self.jamfile_modules = {}

        # Mapping from location to module name
        self.location2module = {}

        # Mapping from project id to project module
        self.id2module = {}

        # Map from Jamfile directory to parent Jamfile/Jamroot
        # location.
        self.dir2parent_jamfile = {}

        # Map from directory to the name of Jamfile in
        # that directory (or None).
        self.dir2jamfile = {}

        # Map from project module to attributes object.
        self.module2attributes = {}

        # Map from project module to target for the project
        self.module2target = {}

        # Map from names to Python modules, for modules loaded
        # via 'using' and 'import' rules in Jamfiles.
        self.loaded_tool_modules_ = {}

        # Map from project target to the list of
        # (id,location) pairs corresponding to all 'use-project'
        # invocations.
        # TODO: should not have a global map, keep this
        # in ProjectTarget.
        self.used_projects = {}

        self.saved_current_project = []

        self.JAMROOT = self.manager.getenv("JAMROOT");

        # Note the use of character groups, as opposed to listing
        # 'Jamroot' and 'jamroot'. With the latter, we'd get duplicate
        # matches on windows and would have to eliminate duplicates.
        if not self.JAMROOT:
            self.JAMROOT = ["project-root.jam", "[Jj]amroot", "[Jj]amroot.jam"]

        # Default patterns to search for the Jamfiles to use for build
        # declarations.
        self.JAMFILE = self.manager.getenv("JAMFILE")

        if not self.JAMFILE:
            self.JAMFILE = ["[Bb]uild.jam", "[Jj]amfile.v2", "[Jj]amfile",
                            "[Jj]amfile.jam"]


    def load (self, jamfile_location):
        """Loads jamfile at the given location. After loading, project global
        file and jamfile needed by the loaded one will be loaded recursively.
        If the jamfile at that location is loaded already, does nothing.
        Returns the project module for the Jamfile."""

        absolute = os.path.join(os.getcwd(), jamfile_location)
        absolute = os.path.normpath(absolute)
        jamfile_location = b2.util.path.relpath(os.getcwd(), absolute)

        if "--debug-loading" in self.manager.argv():
            print "Loading Jamfile at '%s'" % jamfile_location

            
        mname = self.module_name(jamfile_location)
        # If Jamfile is already loaded, don't try again.
        if not mname in self.jamfile_modules:
        
            self.load_jamfile(jamfile_location)
                
            # We want to make sure that child project are loaded only
            # after parent projects. In particular, because parent projects
            # define attributes whch are inherited by children, and we don't
            # want children to be loaded before parents has defined everything.
            #
            # While "build-project" and "use-project" can potentially refer
            # to child projects from parent projects, we don't immediately
            # load child projects when seing those attributes. Instead,
            # we record the minimal information that will be used only later.
            
            self.load_used_projects(mname)
             
        return mname

    def load_used_projects(self, module_name):
        # local used = [ modules.peek $(module-name) : .used-projects ] ;
        used = self.used_projects[module_name]
    
        location = self.attribute(module_name, "location")
        for u in used:
            id = u[0]
            where = u[1]

            self.use(id, os.path.join(location, where))

    def load_parent(self, location):
        """Loads parent of Jamfile at 'location'.
        Issues an error if nothing is found."""

        found = b2.util.path.glob_in_parents(
            location, self.JAMROOT + self.JAMFILE) 

        if not found:
            print "error: Could not find parent for project at '%s'" % location
            print "error: Did not find Jamfile or project-root.jam in any parent directory."
            sys.exit(1)
    
        return self.load(os.path.dirname(found[0]))

    def act_as_jamfile(self, module, location):
        """Makes the specified 'module' act as if it were a regularly loaded Jamfile 
        at 'location'. If Jamfile is already located for that location, it's an 
        error."""

        if self.module_name(location) in self.jamfile_modules:
            self.manager.errors()(
                "Jamfile was already loaded for '%s'" % location)
    
        # Set up non-default mapping from location to module.
        self.location2module[location] = module
    
        # Add the location to the list of project locations
        # so that we don't try to load Jamfile in future
        self.jamfile_modules.append(location)
    
        self.initialize(module, location)

    def find(self, name, current_location):
        """Given 'name' which can be project-id or plain directory name,
        return project module corresponding to that id or directory.
        Returns nothing of project is not found."""

        project_module = None

        # Try interpreting name as project id.
        if name[0] == '/':
            project_module = self.id2module.get(name)

        if not project_module:
            location = os.path.join(current_location, name)
            # If no project is registered for the given location, try to
            # load it. First see if we have Jamfile. If not we might have project
            # root, willing to act as Jamfile. In that case, project-root
            # must be placed in the directory referred by id.
        
            project_module = self.module_name(location)
            if not project_module in self.jamfile_modules and \
               b2.util.path.glob([location], self.JAMROOT + self.JAMFILE):
                project_module = self.load(location)

        return project_module

    def module_name(self, jamfile_location):
        """Returns the name of module corresponding to 'jamfile-location'.
        If no module corresponds to location yet, associates default
        module name with that location."""
        module = self.location2module.get(jamfile_location)
        if not module:
            # Root the path, so that locations are always umbiguious.
            # Without this, we can't decide if '../../exe/program1' and '.'
            # are the same paths, or not.
            jamfile_location = os.path.realpath(
                os.path.join(os.getcwd(), jamfile_location))
            module = "Jamfile<%s>" % jamfile_location
            self.location2module[jamfile_location] = module
        return module

    def find_jamfile (self, dir, parent_root=0, no_errors=0):
        """Find the Jamfile at the given location. This returns the
        exact names of all the Jamfiles in the given directory. The optional
        parent-root argument causes this to search not the given directory
        but the ones above it up to the directory given in it."""
        
        # Glob for all the possible Jamfiles according to the match pattern.
        #
        jamfile_glob = None
        if parent_root:
            parent = self.dir2parent_jamfile.get(dir)
            if not parent:
                parent = b2.util.path.glob_in_parents(dir,
                                                               self.JAMFILE)
                self.dir2parent_jamfile[dir] = parent
            jamfile_glob = parent
        else:
            jamfile = self.dir2jamfile.get(dir)
            if not jamfile:
                jamfile = b2.util.path.glob([dir], self.JAMFILE)
                self.dir2jamfile[dir] = jamfile
            jamfile_glob = jamfile

        if len(jamfile_glob) > 1:
            # Multiple Jamfiles found in the same place. Warn about this.
            # And ensure we use only one of them.
            # As a temporary convenience measure, if there's Jamfile.v2 amount
            # found files, suppress the warning and use it.
            #
            pattern = "(.*[Jj]amfile\\.v2)|(.*[Bb]uild\\.jam)"
            v2_jamfiles = [x for x in jamfile_glob if re.match(pattern, x)]
            if len(v2_jamfiles) == 1:
                jamfile_glob = v2_jamfiles
            else:
                print """warning: Found multiple Jamfiles at '%s'!""" % (dir)
                for j in jamfile_glob:
                    print "    -", j
                print "Loading the first one"
    
        # Could not find it, error.
        if not no_errors and not jamfile_glob:
            self.manager.errors()(
                """Unable to load Jamfile.
Could not find a Jamfile in directory '%s'
Attempted to find it with pattern '%s'.
Please consult the documentation at 'http://boost.org/boost-build2'."""
                % (dir, string.join(self.JAMFILE)))

        return jamfile_glob[0]
    
    def load_jamfile(self, dir):
        """Load a Jamfile at the given directory. Returns nothing.
        Will attempt to load the file as indicated by the JAMFILE patterns.
        Effect of calling this rule twice with the same 'dir' is underfined."""
      
        # See if the Jamfile is where it should be.
        jamfile_to_load = b2.util.path.glob([dir], self.JAMROOT)
        if not jamfile_to_load:
            jamfile_to_load = self.find_jamfile(dir)
        else:
            jamfile_to_load = jamfile_to_load[0]
            
        # The module of the jamfile.
        dir = os.path.realpath(os.path.dirname(jamfile_to_load))
        
        jamfile_module = self.module_name (dir)

        # Initialize the jamfile module before loading.
        #    
        self.initialize(jamfile_module, dir, os.path.basename(jamfile_to_load))

        saved_project = self.current_project

        self.used_projects[jamfile_module] = []
        
        # Now load the Jamfile in it's own context.
        # Initialization might have load parent Jamfiles, which might have
        # loaded the current Jamfile with use-project. Do a final check to make
        # sure it's not loaded already.
        if not jamfile_module in self.jamfile_modules:
            self.jamfile_modules[jamfile_module] = True

            # FIXME:
            # mark-as-user $(jamfile-module) ;

            bjam.call("load", jamfile_module, jamfile_to_load)
            basename = os.path.basename(jamfile_to_load)
                        
        # Now do some checks
        if self.current_project != saved_project:
            self.manager.errors()(
"""The value of the .current-project variable
has magically changed after loading a Jamfile.
This means some of the targets might be defined a the wrong project.
after loading %s
expected value %s
actual value %s""" % (jamfile_module, saved_project, self.current_project))
          
        if self.global_build_dir:
            id = self.attribute(jamfile_module, "id")
            project_root = self.attribute(jamfile_module, "project-root")
            location = self.attribute(jamfile_module, "location")

            if location and project_root == dir:
                # This is Jamroot
                if not id:
                    # FIXME: go via errors module, so that contexts are
                    # shown?
                    print "warning: the --build-dir option was specified"
                    print "warning: but Jamroot at '%s'" % dir
                    print "warning: specified no project id"
                    print "warning: the --build-dir option will be ignored"


    def load_standalone(self, jamfile_module, file):
        """Loads 'file' as standalone project that has no location
        associated with it.  This is mostly useful for user-config.jam,
        which should be able to define targets, but although it has
        some location in filesystem, we don't want any build to
        happen in user's HOME, for example.

        The caller is required to never call this method twice on
        the same file.
        """

        self.initialize(jamfile_module)
        self.used_projects[jamfile_module] = []
        bjam.call("load", jamfile_module, file)
        self.load_used_projects(jamfile_module)
        
    def is_jamroot(self, basename):
        match = [ pat for pat in self.JAMROOT if re.match(pat, basename)]
        if match:
            return 1
        else:
            return 0

    def initialize(self, module_name, location=None, basename=None):
        """Initialize the module for a project.
        
        module-name is the name of the project module.
        location is the location (directory) of the project to initialize.
                 If not specified, stanalone project will be initialized
        """

        if "--debug-loading" in self.manager.argv():
            print "Initializing project '%s'" % module_name

        # TODO: need to consider if standalone projects can do anything but defining
        # prebuilt targets. If so, we need to give more sensible "location", so that
        # source paths are correct.
        if not location:
            location = ""
        else:
            location = b2.util.path.relpath(os.getcwd(), location)

        attributes = ProjectAttributes(self.manager, location, module_name)
        self.module2attributes[module_name] = attributes

        if location:
            attributes.set("source-location", [location], exact=1)
        else:
            attributes.set("source-location", "", exact=1)

        attributes.set("requirements", property_set.empty(), exact=True)
        attributes.set("usage-requirements", property_set.empty(), exact=True)
        attributes.set("default-build", [], exact=True)
        attributes.set("projects-to-build", [], exact=True)
        attributes.set("project-root", None, exact=True)
        attributes.set("build-dir", None, exact=True)
        
        self.project_rules_.init_project(module_name)

        jamroot = False

        parent_module = None;
        if module_name == "site-config":
            # No parent
            pass
        elif module_name == "user-config":
            parent_module = "site-config"
        elif location and not self.is_jamroot(basename):
            # We search for parent/project-root only if jamfile was specified 
            # --- i.e
            # if the project is not standalone.
            parent_module = self.load_parent(location)
        else:
            # It's either jamroot, or standalone project.
            # If it's jamroot, inherit from user-config.
            if location:
                parent_module = "user-config" ;                
                jamroot = True ;
                
        if parent_module:
            self.inherit_attributes(module_name, parent_module)
            attributes.set("parent-module", parent_module, exact=1)

        if jamroot:
            attributes.set("project-root", location, exact=1)
                                
        parent = None
        if parent_module:
            parent = self.target(parent_module)

        if not self.module2target.has_key(module_name):
            target = b2.build.targets.ProjectTarget(self.manager,
                module_name, module_name, parent,
                self.attribute(module_name,"requirements"),
                # FIXME: why we need to pass this? It's not
                # passed in jam code.
                self.attribute(module_name, "default-build"))
            self.module2target[module_name] = target

        self.current_project = self.target(module_name)

    def inherit_attributes(self, project_module, parent_module):
        """Make 'project-module' inherit attributes of project
        root and parent module."""

        attributes = self.module2attributes[project_module]
        pattributes = self.module2attributes[parent_module]
        
        # Parent module might be locationless user-config.
        # FIXME:
        #if [ modules.binding $(parent-module) ]
        #{        
        #    $(attributes).set parent : [ path.parent 
        #                                 [ path.make [ modules.binding $(parent-module) ] ] ] ;
        #    }
        
        attributes.set("project-root", pattributes.get("project-root"), exact=True)
        attributes.set("default-build", pattributes.get("default-build"), exact=True)
        attributes.set("requirements", pattributes.get("requirements"), exact=True)
        attributes.set("usage-requirements",
                       pattributes.get("usage-requirements"), exact=1)

        parent_build_dir = pattributes.get("build-dir")
        
        if parent_build_dir:
        # Have to compute relative path from parent dir to our dir
        # Convert both paths to absolute, since we cannot
        # find relative path from ".." to "."

             location = attributes.get("location")
             parent_location = pattributes.get("location")

             our_dir = os.path.join(os.getcwd(), location)
             parent_dir = os.path.join(os.getcwd(), parent_location)

             build_dir = os.path.join(parent_build_dir,
                                      b2.util.path.relpath(parent_dir,
                                                                    our_dir))

    def register_id(self, id, module):
        """Associate the given id with the given project module."""
        self.id2module[id] = module

    def current(self):
        """Returns the project which is currently being loaded."""
        return self.current_project

    def push_current(self, project):
        """Temporary changes the current project to 'project'. Should
        be followed by 'pop-current'."""
        self.saved_current_project.append(self.current_project)
        self.current_project = project

    def pop_current(self):
        self.current_project = self.saved_current_project[-1]
        del self.saved_current_project[-1]

    def attributes(self, project):
        """Returns the project-attribute instance for the
        specified jamfile module."""
        return self.module2attributes[project]

    def attribute(self, project, attribute):
        """Returns the value of the specified attribute in the
        specified jamfile module."""
        return self.module2attributes[project].get(attribute)
        try:    
            return self.module2attributes[project].get(attribute)
        except:
            print "Sucks", project, attribute
            raise "Sucks"

    def attributeDefault(self, project, attribute, default):
        """Returns the value of the specified attribute in the
        specified jamfile module."""
        return self.module2attributes[project].getDefault(attribute, default)

    def target(self, project_module):
        """Returns the project target corresponding to the 'project-module'."""
        if not self.module2target.has_key(project_module):
            self.module2target[project_module] = \
                b2.build.targets.ProjectTarget(project_module, project_module,
                              self.attribute(project_module, "requirements"))
        
        return self.module2target[project_module]

    def use(self, id, location):
        # Use/load a project.
        saved_project = self.current_project
        project_module = self.load(location)
        declared_id = self.attributeDefault(project_module, "id", "")

        if not declared_id or declared_id != id:
            # The project at 'location' either have no id or
            # that id is not equal to the 'id' parameter.
            if self.id2module.has_key(id) and self.id2module[id] != project_module:
                self.manager.errors()(
"""Attempt to redeclare already existing project id '%s'""" % id)
            self.id2module[id] = project_module

        self.current_module = saved_project

    def add_rule(self, name, callable):
        """Makes rule 'name' available to all subsequently loaded Jamfiles.

        Calling that rule wil relay to 'callable'."""
        self.project_rules_.add_rule(name, callable)

    def project_rules(self):
        return self.project_rules_

    def glob_internal(self, project, wildcards, excludes, rule_name):
        location = project.get("source-location")

        result = []
        callable = b2.util.path.__dict__[rule_name]
        
        paths = callable(location, wildcards, excludes)
        has_dir = 0
        for w in wildcards:
            if os.path.dirname(w):
                has_dir = 1
                break

        if has_dir or rule_name != "glob":
            # The paths we've found are relative to current directory,
            # but the names specified in sources list are assumed to
            # be relative to source directory of the corresponding
            # prject. So, just make the name absolute.
            result = [os.path.join(os.getcwd(), p) for p in paths]
        else:
            # There were not directory in wildcard, so the files are all
            # in the source directory of the project. Just drop the
            # directory, instead of making paths absolute.
            result = [os.path.basename(p) for p in paths]
            
        return result

    def load_module(self, name, extra_path=None):
        """Classic Boost.Build 'modules' are in fact global variables.
        Therefore, try to find an already loaded Python module called 'name' in sys.modules. 
        If the module ist not loaded, find it Boost.Build search
        path and load it.  The new module is not entered in sys.modules.
        The motivation here is to have disjoint namespace of modules
        loaded via 'import/using' in Jamfile, and ordinary Python
        modules. We don't want 'using foo' in Jamfile to load ordinary
        Python module 'foo' which is going to not work. And we
        also don't want 'import foo' in regular Python module to
        accidentally grab module named foo that is internal to
        Boost.Build and intended to provide interface to Jamfiles."""

        existing = self.loaded_tool_modules_.get(name)
        if existing:
            return existing

        modules = sys.modules
        for class_name in modules:
            if name is class_name:
                module = modules[class_name]
                self.loaded_tool_modules_[name] = module
                return module
        
        path = extra_path
        if not path:
            path = []
        path.extend(self.manager.boost_build_path())
        location = None
        for p in path:
            l = os.path.join(p, name + ".py")
            if os.path.exists(l):
                location = l
                break

        if not location:
            self.manager.errors()("Cannot find module '%s'" % name)

        mname = "__build_build_temporary__"
        file = open(location)
        try:
            # TODO: this means we'll never make use of .pyc module,
            # which might be a problem, or not.
            module = imp.load_module(mname, file, os.path.basename(location),
                                     (".py", "r", imp.PY_SOURCE))
            del sys.modules[mname]
            self.loaded_tool_modules_[name] = module
            return module
        finally:
            file.close()
        


# FIXME:
# Defines a Boost.Build extension project. Such extensions usually
# contain library targets and features that can be used by many people.
# Even though extensions are really projects, they can be initialize as
# a module would be with the "using" (project.project-rules.using)
# mechanism.
#rule extension ( id : options * : * )
#{
#    # The caller is a standalone module for the extension.
#    local mod = [ CALLER_MODULE ] ;
#    
#    # We need to do the rest within the extension module.
#    module $(mod)
#    {
#        import path ;
#        
#        # Find the root project.
#        local root-project = [ project.current ] ;
#        root-project = [ $(root-project).project-module ] ;
#        while
#            [ project.attribute $(root-project) parent-module ] &&
#            [ project.attribute $(root-project) parent-module ] != user-config
#        {
#            root-project = [ project.attribute $(root-project) parent-module ] ;
#        }
#        
#        # Create the project data, and bring in the project rules
#        # into the module.
#        project.initialize $(__name__) :
#            [ path.join [ project.attribute $(root-project) location ] ext $(1:L) ] ;
#        
#        # Create the project itself, i.e. the attributes.
#        # All extensions are created in the "/ext" project space.
#        project /ext/$(1) : $(2) : $(3) : $(4) : $(5) : $(6) : $(7) : $(8) : $(9) ;
#        local attributes = [ project.attributes $(__name__) ] ;
#        
#        # Inherit from the root project of whomever is defining us.
#        project.inherit-attributes $(__name__) : $(root-project) ;
#        $(attributes).set parent-module : $(root-project) : exact ;
#    }
#}
        

class ProjectAttributes:
    """Class keeping all the attributes of a project.

    The standard attributes are 'id', "location", "project-root", "parent"
    "requirements", "default-build", "source-location" and "projects-to-build".
    """
        
    def __init__(self, manager, location, project_module):
        self.manager = manager
        self.location = location
        self.project_module = project_module
        self.attributes = {}
        self.usage_requirements = None
        
    def set(self, attribute, specification, exact):
        """Set the named attribute from the specification given by the user.
        The value actually set may be different."""

        if exact:
            self.__dict__[attribute] = specification
            
        elif attribute == "requirements":
            self.requirements = property_set.refine_from_user_input(
                self.requirements, specification,
                self.project_module, self.location)
            
        elif attribute == "usage-requirements":
            unconditional = []
            for p in specification:
                split = property.split_conditional(p)
                if split:
                    unconditional.append(split[1])
                else:
                    unconditional.append(p)

            non_free = property.remove("free", unconditional)
            if non_free:                
                pass
                # FIXME:
                #errors.error "usage-requirements" $(specification) "have non-free properties" $(non-free) ;

            t = property.translate_paths(specification, self.location)

            existing = self.__dict__.get("usage-requirements")
            if existing:
                new = property_set.create(existing.raw() +  t)
            else:
                new = property_set.create(t)
            self.__dict__["usage-requirements"] = new

                
        elif attribute == "default-build":
            self.__dict__["default-build"] = property_set.create(specification)
            
        elif attribute == "source-location":
            source_location = []
            for path in specification:
                source_location += os.path.join(self.location, path)
            self.__dict__["source-location"] = source_location
                
        elif attribute == "build-dir":
            self.__dict__["build-dir"] = os.path.join(self.location, specification)
                 
        elif not attribute in ["id", "default-build", "location",
                               "source-location", "parent",
                               "projects-to-build", "project-root"]:
            self.manager.errors()(
"""Invalid project attribute '%s' specified
for project at '%s'""" % (attribute, self.location))
        else:
            self.__dict__[attribute] = specification

    def get(self, attribute):
        return self.__dict__[attribute]

    def getDefault(self, attribute, default):
        return self.__dict__.get(attribute, default)

    def dump(self):
        """Prints the project attributes."""
        id = self.get("id")
        if not id:
            id = "(none)"
        else:
            id = id[0]

        parent = self.get("parent")
        if not parent:
            parent = "(none)"
        else:
            parent = parent[0]

        print "'%s'" % id
        print "Parent project:%s", parent
        print "Requirements:%s", self.get("requirements")
        print "Default build:%s", string.join(self.get("debuild-build"))
        print "Source location:%s", string.join(self.get("source-location"))
        print "Projects to build:%s", string.join(self.get("projects-to-build").sort());

class ProjectRules:
    """Class keeping all rules that are made available to Jamfile."""

    def __init__(self, registry):
        self.registry = registry
        self.manager_ = registry.manager
        self.rules = {}
        self.local_names = [x for x in self.__class__.__dict__
                            if x not in ["__init__", "init_project", "add_rule",
                                         "error_reporting_wrapper", "add_rule_for_type"]]
        self.all_names_ = [x for x in self.local_names]

    def add_rule_for_type(self, type):
        rule_name = type.lower();

        def xpto (name, sources, requirements = [], default_build = None, usage_requirements = []):
            return self.manager_.targets().create_typed_target(
                type, self.registry.current(), name[0], sources,
                requirements, default_build, usage_requirements) 

        self.add_rule(type.lower(), xpto)
    
    def add_rule(self, name, callable):
        self.rules[name] = callable
        self.all_names_.append(name)

    def all_names(self):
        return self.all_names_

    def call_and_report_errors(self, callable, *args):
        result = None
        try:
            self.manager_.errors().push_jamfile_context()
            result = callable(*args)
        except ExceptionWithUserContext, e:
            e.report()
        except Exception, e:
            try:
                self.manager_.errors().handle_stray_exception (e)
            except ExceptionWithUserContext, e:
                e.report()
        finally:                
            self.manager_.errors().pop_jamfile_context()
                                        
        return result

    def make_wrapper(self, callable):
        """Given a free-standing function 'callable', return a new
        callable that will call 'callable' and report all exceptins,
        using 'call_and_report_errors'."""
        def wrapper(*args):
            self.call_and_report_errors(callable, *args)
        return wrapper

    def init_project(self, project_module):

        for n in self.local_names:            
            # Using 'getattr' here gives us a bound method,
            # while using self.__dict__[r] would give unbound one.
            v = getattr(self, n)
            if callable(v):
                if n == "import_":
                    n = "import"
                else:
                    n = string.replace(n, "_", "-")
                    
                bjam.import_rule(project_module, n,
                                 self.make_wrapper(v))

        for n in self.rules:
            bjam.import_rule(project_module, n,
                             self.make_wrapper(self.rules[n]))

    def project(self, *args):

        jamfile_module = self.registry.current().project_module()
        attributes = self.registry.attributes(jamfile_module)
        
        id = None
        if args and args[0]:
            id = args[0][0]
            args = args[1:]

        if id:
            if id[0] != '/':
                id = '/' + id
            self.registry.register_id (id, jamfile_module)

        explicit_build_dir = None
        for a in args:
            if a:
                attributes.set(a[0], a[1:], exact=0)
                if a[0] == "build-dir":
                    explicit_build_dir = a[1]
        
        # If '--build-dir' is specified, change the build dir for the project.
        if self.registry.global_build_dir:

            location = attributes.get("location")
            # Project with empty location is 'standalone' project, like
            # user-config, or qt.  It has no build dir.
            # If we try to set build dir for user-config, we'll then
            # try to inherit it, with either weird, or wrong consequences.
            if location and location == attributes.get("project-root"):
                # This is Jamroot.
                if id:
                    if explicit_build_dir and os.path.isabs(explicit_build_dir):
                        self.register.manager.errors()(
"""Absolute directory specified via 'build-dir' project attribute
Don't know how to combine that with the --build-dir option.""")

                    rid = id
                    if rid[0] == '/':
                        rid = rid[1:]

                    p = os.path.join(self.registry.global_build_dir,
                                     rid, explicit_build_dir)
                    attributes.set("build-dir", p, exact=1)
            elif explicit_build_dir:
                self.registry.manager.errors()(
"""When --build-dir is specified, the 'build-dir'
attribute is allowed only for top-level 'project' invocations""")

    def constant(self, name, value):
        """Declare and set a project global constant.
        Project global constants are normal variables but should
        not be changed. They are applied to every child Jamfile."""
        m = "Jamfile</home/ghost/Work/Boost/boost-svn/tools/build/v2_python/python/tests/bjam/make>"
        self.registry.current().add_constant(name[0], value)

    def path_constant(self, name, value):
        """Declare and set a project global constant, whose value is a path. The
        path is adjusted to be relative to the invocation directory. The given
        value path is taken to be either absolute, or relative to this project
        root."""
        self.registry.current().add_constant(name[0], value, path=1)

    def use_project(self, id, where):
        # See comment in 'load' for explanation why we record the
        # parameters as opposed to loading the project now.
        m = self.registry.current().project_module();
        self.registry.used_projects[m].append((id[0], where[0]))
        
    def build_project(self, dir):
        assert(isinstance(dir, list))
        jamfile_module = self.registry.current().project_module()
        attributes = self.registry.attributes(jamfile_module)
        now = attributes.get("projects-to-build")
        attributes.set("projects-to-build", now + dir, exact=True)

    def explicit(self, target_names):
        t = self.registry.current()
        for n in target_names:
            t.mark_target_as_explicit(n)

    def glob(self, wildcards, excludes=None):
        return self.registry.glob_internal(self.registry.current(),
                                           wildcards, excludes, "glob")

    def glob_tree(self, wildcards, excludes=None):
        bad = 0
        for p in wildcards:
            if os.path.dirname(p):
                bad = 1

        if excludes:
            for p in excludes:
                if os.path.dirname(p):
                    bad = 1

        if bad:
            self.registry.manager().errors()(
"The patterns to 'glob-tree' may not include directory")
        return self.registry.glob_internal(self.registry.current(),
                                           wildcards, excludes, "glob_tree")
    

    def using(self, toolset, *args):
        # The module referred by 'using' can be placed in
        # the same directory as Jamfile, and the user
        # will expect the module to be found even though
        # the directory is not in BOOST_BUILD_PATH.
        # So temporary change the search path.
        jamfile_module = self.registry.current().project_module()
        attributes = self.registry.attributes(jamfile_module)
        location = attributes.get("location")

        m = self.registry.load_module(toolset[0], [location])
        if not m.__dict__.has_key("init"):
            self.registry.manager.errors()(
                "Tool module '%s' does not define the 'init' method" % toolset[0])
        m.init(*args)


    def import_(self, name, names_to_import=None, local_names=None):

        name = name[0]
        jamfile_module = self.registry.current().project_module()
        attributes = self.registry.attributes(jamfile_module)
        location = attributes.get("location")

        m = self.registry.load_module(name, [location])

        for f in m.__dict__:
            v = m.__dict__[f]
            if callable(v):
                bjam.import_rule(jamfile_module, name + "." + f, v)

        if names_to_import:
            if not local_names:
                local_names = names_to_import

            if len(names_to_import) != len(local_names):
                self.registry.manager.errors()(
"""The number of names to import and local names do not match.""")

            for n, l in zip(names_to_import, local_names):
                bjam.import_rule(jamfile_module, l, m.__dict__[n])
        
    def conditional(self, condition, requirements):
        """Calculates conditional requirements for multiple requirements
        at once. This is a shorthand to be reduce duplication and to
        keep an inline declarative syntax. For example:

            lib x : x.cpp : [ conditional <toolset>gcc <variant>debug :
                <define>DEBUG_EXCEPTION <define>DEBUG_TRACE ] ;
        """

        c = string.join(condition, ",")
        if c.find(":") != -1:
            return [c + r for r in requirements]
        else:
            return [c + ":" + r for r in requirements]