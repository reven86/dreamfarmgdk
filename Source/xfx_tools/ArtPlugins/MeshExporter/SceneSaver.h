#ifndef __SCENE_SAVER__
#define __SCENE_SAVER__





//-----------------------------------------------------------------------------
//                           Classes
//-----------------------------------------------------------------------------

//
// Scene saver
//

class SceneSaver : public ITreeEnumProc
{
	struct Vertex
	{
		xfx::Vec3		pos;
		xfx::Vec3		norm;
		xfx::ARGB		diffuse;

		float tu[8], tv[8], tw[8];

		friend inline bool operator ==			(const Vertex& a, const Vertex& b) {return !memcmp (&a, &b, sizeof (Vertex));};
	};

	std::vector<Vertex>					mVertices;
	std::vector<DWORD>					mIndices;

	Interface							* mpInterface;

	const class Options					* mpOptions;

public:
	SceneSaver															();
	virtual ~SceneSaver													();

	void								Save							(const TCHAR * name) const;
	void								Reset							(Interface * ip, const class Options * o);
	virtual int							callback						(INode * node);

private:
	void								ProcessNode						(INode * node);
	void								ExportMesh						(TriObject * obj, const Matrix3& tm);
	DWORD								AppendVertex					(const Vertex& v);

	//Return a pointer to a TriObject given an INode or return NULL
	//if the node cannot be converted to a TriObject
	TriObject *							GetTriObjectFromNode			(INode * node, int& delete_it) const;
	Point3								GetVertexNormal					(TriObject * tri, const DWORD& i, const DWORD& faceindex, const DWORD& smgroup) const;

	void								WriteVertex						(const Vertex& v, FILE * f) const;
	void								WriteIndex						(const DWORD& i, FILE * f) const;
};



#endif