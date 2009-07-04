#include "pch.h"
#include "SceneSaver.h"
#include "Options.h"





//-----------------------------------------------------------------------------
//                           Methods
//-----------------------------------------------------------------------------

//SceneSaver
SceneSaver::SceneSaver () :
	mpInterface (NULL)
{
}

SceneSaver::~SceneSaver ()
{
}

void SceneSaver::Reset (Interface * ip, const Options * o)
{
	mpInterface = ip;
	mpOptions = o;

	mVertices.clear ();
	mIndices.clear ();
}

int SceneSaver::callback (INode * node)
{
	if (!node->IsNodeHidden ())
		ProcessNode (node);
	
	return TREE_CONTINUE;
}

void SceneSaver::ProcessNode (INode * node)
{
	TriObject * tri;
	int del;

	tri = GetTriObjectFromNode (node, del);
	if (!tri) return;

	ExportMesh (tri, node->GetObjTMAfterWSM (mpInterface->GetTime ()));

	if (del)
		tri->DeleteMe ();
}

void SceneSaver::ExportMesh (TriObject * obj, const Matrix3& tm)
{
	Matrix3 tm_notrans (tm);
	tm_notrans.NoTrans ();

	obj->mesh.buildNormals ();

	for (int i = 0; i < obj->mesh.numFaces; i++)
	{
		DWORD i1 = obj->mesh.faces[i].getVert (0);
		DWORD i2 = obj->mesh.faces[i].getVert (2);
		DWORD i3 = obj->mesh.faces[i].getVert (1);

		Vertex v1, v2, v3;

		v1.pos = ToVector (tm * obj->mesh.verts[i1]);
		v2.pos = ToVector (tm * obj->mesh.verts[i2]);
		v3.pos = ToVector (tm * obj->mesh.verts[i3]);

		DWORD smgroup = obj->mesh.faces[i].getSmGroup ();
		v1.norm = ToVector ((tm_notrans * GetVertexNormal (obj, i1, i, smgroup)).Normalize ());
		v2.norm = ToVector ((tm_notrans * GetVertexNormal (obj, i2, i, smgroup)).Normalize ());
		v3.norm = ToVector ((tm_notrans * GetVertexNormal (obj, i3, i, smgroup)).Normalize ());

		for (int j = 0; j < 8; j++)
			if (obj->mesh.mapSupport (j + 1) && obj->mesh.getNumMapVerts (j + 1))
			{
				DWORD ti1 = (obj->mesh.mapFaces (j + 1) + i)->t[0];
				DWORD ti2 = (obj->mesh.mapFaces (j + 1) + i)->t[2];
				DWORD ti3 = (obj->mesh.mapFaces (j + 1) + i)->t[1];

				v1.tu[j] =  (obj->mesh.mapVerts (j + 1) + ti1)->x;
				v1.tv[j] = -(obj->mesh.mapVerts (j + 1) + ti1)->y;
				v1.tw[j] =  (obj->mesh.mapVerts (j + 1) + ti1)->z;
				v2.tu[j] =  (obj->mesh.mapVerts (j + 1) + ti2)->x;
				v2.tv[j] = -(obj->mesh.mapVerts (j + 1) + ti2)->y;
				v2.tw[j] =  (obj->mesh.mapVerts (j + 1) + ti2)->z;
				v3.tu[j] =  (obj->mesh.mapVerts (j + 1) + ti3)->x;
				v3.tv[j] = -(obj->mesh.mapVerts (j + 1) + ti3)->y;
				v3.tw[j] =  (obj->mesh.mapVerts (j + 1) + ti3)->z;
			}
			else
			{
				v1.tu[j] = v1.tv[j] = v1.tw[j] =
				v2.tu[j] = v2.tv[j] = v2.tw[j] =
				v3.tu[j] = v3.tv[j] = v3.tw[j] = 0.0f;
			}

		if (obj->mesh.mapSupport (0) && obj->mesh.getNumMapVerts (0))
		{
			DWORD ti1 = (obj->mesh.mapFaces (0) + i)->t[0];
			DWORD ti2 = (obj->mesh.mapFaces (0) + i)->t[2];
			DWORD ti3 = (obj->mesh.mapFaces (0) + i)->t[1];

			v1.diffuse = xfx::ARGB (255,
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti1)->x * 255.0f),
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti1)->y * 255.0f),
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti1)->z * 255.0f)
				);

			v2.diffuse = xfx::ARGB (255,
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti2)->x * 255.0f),
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti2)->y * 255.0f),
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti2)->z * 255.0f)
				);

			v3.diffuse = xfx::ARGB (255,
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti3)->x * 255.0f),
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti3)->y * 255.0f),
				static_cast<BYTE> ((obj->mesh.mapVerts (0) + ti3)->z * 255.0f)
				);
		}
		else
		{
			v1.diffuse = v2.diffuse = v3.diffuse = xfx::ARGB (0xffffffff);
		}

		mIndices.push_back (AppendVertex (v1));
		mIndices.push_back (AppendVertex (v2));
		mIndices.push_back (AppendVertex (v3));
	}
}

DWORD SceneSaver::AppendVertex (const Vertex& v)
{
	std::vector<Vertex>::iterator it = std::find (mVertices.begin (), mVertices.end (), v);

	if (it == mVertices.end ())
	{
		mVertices.push_back (v);
		return static_cast<DWORD> (mVertices.size ()) - 1;
	}

	return static_cast<DWORD> (std::distance (mVertices.begin (), it));
}

TriObject * SceneSaver::GetTriObjectFromNode (INode * node, int& delete_it) const
{
	delete_it = FALSE;

	Object * obj = node->EvalWorldState (mpInterface->GetTime ()).obj;

	if (obj->CanConvertToType (Class_ID (TRIOBJ_CLASS_ID, 0)))
	{ 
		TriObject * tri = static_cast<TriObject *> (obj->ConvertToType (mpInterface->GetTime(), Class_ID (TRIOBJ_CLASS_ID, 0)));

		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()

		if (obj != tri) delete_it = TRUE;
  		return tri;
	}

	return NULL;
}

Point3 SceneSaver::GetVertexNormal (TriObject * obj, const DWORD& i, const DWORD& faceindex, const DWORD& smgroup) const
{
	RVertex& rvert = obj->mesh.getRVert (i);

	DWORD normals = rvert.rFlags & NORCT_MASK;

	if (!normals || !smgroup)
	{
		return obj->mesh.getFaceNormal (faceindex);
	}
	else if ((normals == 1) || (rvert.rFlags & SPECIFIED_NORMAL))
	{
		return rvert.rn.getNormal ();
	}

	for (DWORD i = 0; i < normals; i++)
		if (rvert.ern[i].getSmGroup () & smgroup)
		{
			return rvert.ern[i].getNormal ();
		}

	return Point3 (0.0f, 0.0f, 0.0f);
}

void SceneSaver::Save (const TCHAR * name) const
{
	FILE * file = fopen (name, "wb");
	if (!file)
	{
		MessageBox (NULL, "Can't create file!", "Error", MB_OK);
		return;
	}

	DWORD sign = MAKEFOURCC ('M', 'E', 'S', 'H');
	fwrite (&sign, sizeof (sign), 1, file);

	const DWORD MODEL_FILE_VER = 100;
	fwrite (&MODEL_FILE_VER, sizeof (MODEL_FILE_VER), 1, file);

	DWORD fvf = mpOptions->MakeFVF ();
	fwrite (&fvf, sizeof (fvf), 1, file);

	DWORD vert_size = mpOptions->GetVertexSize ();
	fwrite (&vert_size, sizeof (vert_size), 1, file);

	DWORD vert_count	= static_cast<DWORD> (mVertices.size ());
	DWORD ind_count		= static_cast<DWORD> (mIndices.size ());
	DWORD faces_count	= ind_count / 3;

	DWORD flags = ( mpOptions->ExportFlags () & Options::EX_SHORT_INDICES ) != 0 ? 1 : 0;

	fwrite (&flags, sizeof (flags), 1, file);
	fwrite (&vert_count, sizeof (vert_count), 1, file);
	fwrite (&faces_count, sizeof (faces_count), 1, file);

	std::for_each (
		mVertices.begin (),
		mVertices.end (),
		boost::bind (&SceneSaver::WriteVertex, this, _1, file)
		);

	std::for_each (
		mIndices.begin (),
		mIndices.end (),
		boost::bind (&SceneSaver::WriteIndex, this, _1, file)
		);

	fclose (file);
}

void SceneSaver::WriteVertex (const Vertex& v, FILE * f) const
{
	fwrite (&v.pos.x, sizeof (v.pos.x), 1, f);
	fwrite (&v.pos.z, sizeof (v.pos.z), 1, f);
	fwrite (&v.pos.y, sizeof (v.pos.y), 1, f);

	if (mpOptions->ExportFlags () & Options::EX_NORMAL)
	{
		fwrite (&v.norm.x, sizeof (v.norm.x), 1, f);
		fwrite (&v.norm.z, sizeof (v.norm.z), 1, f);
		fwrite (&v.norm.y, sizeof (v.norm.y), 1, f);
	}

	if (mpOptions->ExportFlags () & Options::EX_DIFFUSE)
	{
		fwrite (&v.diffuse.dword, sizeof (v.diffuse.dword), 1, f);
	}

	for (int i = 0; i < 8; i++)
		switch (mpOptions->TexCoordSize (i))
		{
		case 0:
			break;

		case 1:
			fwrite (&v.tu[i], sizeof (v.tu[i]), 1, f);
			break;

		case 3:
			fwrite (&v.tu[i], sizeof (v.tu[i]), 1, f);
			fwrite (&v.tv[i], sizeof (v.tv[i]), 1, f);
			fwrite (&v.tw[i], sizeof (v.tw[i]), 1, f);
			break;

		default:
			fwrite (&v.tu[i], sizeof (v.tu[i]), 1, f);
			fwrite (&v.tv[i], sizeof (v.tv[i]), 1, f);
			break;
		}
}

void SceneSaver::WriteIndex (const DWORD& i, FILE * f) const
{
	if (mpOptions->ExportFlags () & Options::EX_SHORT_INDICES)
	{
		USHORT s = static_cast<USHORT> (i);
		fwrite (&s, sizeof (s), 1, f);
	}
	else
		fwrite (&i, sizeof (i), 1, f);
}