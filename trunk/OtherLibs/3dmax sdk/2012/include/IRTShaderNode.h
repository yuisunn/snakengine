/**********************************************************************
*<
FILE:			IRTShaderNode.h

DESCRIPTION:	Interface for a shader node as part of the shade tree
				including Management

CREATED BY:		Neil Hazzard

HISTORY:		created March 8th, 2007

*>	Copyright (c) 2007, All Rights Reserved.
**********************************************************************/
#pragma once
#include <WTypes.h>
#include "export.h"
#include "MaxHeap.h"
#include "strbasic.h"

// forward declarations
class Animatable;
class INode;
class Mesh;
class MNMesh;


//!\brief A Hardware shader node for use with compiled shader trees
/*! IRTShaderNode is an interface to a node that has its visual representation handled by an internal
shade tree generated by a compiler technology such as AMD's Ashli or Mental Images Mental Mill technology. 
The interface wraps many of the draw calls, shader and state management, including cache creation for these 
complex shaders.  It would typically be used with in a Material, but for 3ds max 2008, it was extended to 
support background nodes.  Internally the system implements the IRenderMesh interfaces.  A developer can 
create a unique node by using methods of IRTShaderManager, in particular AddShaderNode.  This interface 
does not remove the need to implement IDX9VertexShader interfaces, but simply allows passing control of 
certain methods to the IRTShaderNode interfaces.  Currently only the Standard Material and the Architectural 
Material participate in these shade trees.\n
NB - This system is only valid for DirectX Viewports - it will not be active on openGL or Heidi.
*/
class IRTShaderNode : public MaxHeapOperators
{
public:
	/*! Assign the current active mesh and node data to the shading sub system.  This would be 
	implemented in IDX9VertexShader::Initialize(..)
	\param[in] node The node to render
	\param[in] mesh The mesh to render
	\return TRUE if the data was successfully allocated.  FALSE would indicate an internal failure.
	*/
	virtual bool SetRenderObjects(INode * node, Mesh * mesh)=0;

	/*! Assign the current active MNMesh and node data to the shading sub system.  This would be 
	implemented in IDX9VertexShader::Initialize(..)
	\param[in] node The node to render
	\param[in] mnmesh The mesh to render
	\return TRUE if the data was successfully allocated.  FALSE would indicate an internal failure.
	*/
	virtual bool SetRenderObjects(INode * node, MNMesh * mnmesh)=0;

	/*! Request the rendering of the assigned shade tree.  The calling code has no control over what is rendered
	it simple initiates the draw call.  This would typically be called in response to the 
	IDX9VertexShader::DrawMeshStrips().  The return code can be passed directly back to DrawMeshStrips.  FALSE 
	would indicate that the object(s) was not drawn.
	\return TRUE if the objects are rendered, false if an error was raised.  Returning false to DrawMeshStrips would
	allow the GFX layer  to continue to render the objects - returning TRUE would indicated that no further processing
	is required.
	*/
	virtual bool DrawObjects() =0;

	/*! The underlying system requires access to key viewport interfaces.  This method allows the shader node to assign
	the current window interface.  It is passed as a LPVOID so as to assist working with DirectX9 and DirectX10 interfaces.
	The underlying system knows which sub system is running, but can't access the viewport directly.  To assign a DirectX9
	window simply cast an ID3D9GraphicsWindow obtained from IDX9VertexShader::ConfirmDevice.  For DirectX10 ID3D10GraphicsWindow
	would be used instead.
	\param[in] gfxWindow A pointer to the current active hardware viewport
	*/
	virtual void RegisterGraphicsWindow(LPVOID gfxWindow)=0;
};

//!\brief A simple public manager for IRTShaderNode
/*! A simple public manager that creates and maintains IRTShaderNodes used by materials and other nodes in max.  All creation
and deletion of the objects is performed by the manager.  The developer simply requests a new IRTShaderNode and releases it
when finished.  Internally the manager is responsible for maintaining the validity of the nodes, and performing Lost Devices
routines on all nodes.  It implements IEffectManager and manages all calls to the IEffectParser interfaces.
*/
class IRTShaderManager: public MaxHeapOperators
{
public:
	/*! Register a new IRTShaderNode.  THe developer can keep hold of this pointer until they release it.  The Animatable
	pointer is used to look up the data in various methods internally and also the FindShaderNode.
	\param[in] anim The associated Animatable for the render node
	\return A pointer to a new IRTShaderNode object.
	*/
	virtual IRTShaderNode * AddShaderNode(Animatable* anim) =0;

	/*! When the animatable (e.g. a material) is being deleted the IRTShaderNode should be removed from the active list
	\param[in] shadeNode The node to remove.
	*/
	virtual void RemoveShaderNode(IRTShaderNode* shadeNode)=0;

	/*! Given an Animatable the associated IRTShaderNode can be retrieved.  An example is the background image display.  
	A Bitmap can be displayed in the viewport background, so max checks to see if the currently assigned Texmap has a shader
	node registered.  If it does it will use this to render the background, instead of the default software processing
	\param[in] anim The Animatable to query
	\return The corresponding shader node, or NULL if nothing is found
	*/
	virtual IRTShaderNode * FindShaderNode(Animatable * anim)=0;


	/*! Access to the IRTShaderManager interface.  A developer would use something like the following.
	\code
	IRTShaderManager * lpShaderMgr = IRTShaderManager::GetIRTShaderManager()
	\endcode
	It is exported in the RTMax.dll
	\return Access to the IRTShaderManager singleton
	*/
	DllExport static IRTShaderManager * GetRTShaderManager();
};

//! \brief An extension to the IRTShaderManager interface
class IRTShaderManager2 : public IRTShaderManager
{

public:
	/*! For the given Animatable access the compiled effect code from the system.  THis will be the actual
	code generated by the tree compiler, and passed to the DirectX frame work.
	\param[in] anim The animatable whose shader code is requested
	\return A buffer containing the effect (shader) code.  The pointer should not be stored as the shader node responsible for
	maintaining the buffer could be destroyed.
	*/
	virtual const MCHAR * GetShaderCode(Animatable * anim)=0;

	/*! Access to the IRTShaderManager2 interface.  A developer would use something like the following.
	\code
	IRTShaderManager2 * lpShaderMgr2 = IRTShaderManager2::GetIRTShaderManager2()
	\endcode
	It is exported in the RTMax.dll
	\return Access to the IRTShaderManager2 singleton
	*/
	DllExport static IRTShaderManager2 * GetRTShaderManager2();

};