/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "CmGpuProgramManager.h"
//#include "CmHighLevelGpuProgramManager.h"
#include "CmRenderSystem.h"
#include "CmRenderSystemManager.h"


namespace CamelotEngine {
	//---------------------------------------------------------------------------
	GpuProgramManager::GpuProgramManager()
	{
		// subclasses should register with resource group manager
	}
	//---------------------------------------------------------------------------
	GpuProgramManager::~GpuProgramManager()
	{
		// subclasses should unregister with resource group manager
	}
    //---------------------------------------------------------------------------
	GpuProgramPtr GpuProgramManager::load(const String& code, 
        GpuProgramType gptype, const String& syntaxCode)
    {
		GpuProgramPtr prg;
		{
			CM_LOCK_AUTO_MUTEX
			prg = createProgram( code, gptype, syntaxCode);
		}
        prg->load();
        return prg;
    }
    //---------------------------------------------------------------------------
	GpuProgramPtr GpuProgramManager::createProgram(const String& code, GpuProgramType gptype, 
		const String& syntaxCode)
    {
		GpuProgramPtr prg = GpuProgramPtr(create(gptype, syntaxCode));
        // Set all prarmeters (create does not set, just determines factory)
		prg->setType(gptype);
		prg->setSyntaxCode(syntaxCode);
		prg->setSource(code);
        return prg;
    }
    //---------------------------------------------------------------------------
		const GpuProgramManager::SyntaxCodes& GpuProgramManager::getSupportedSyntax(void) const
        {
			// Use the current render system
			RenderSystem* rs = CamelotEngine::RenderSystemManager::getActive();

			// Get the supported syntaxed from RenderSystemCapabilities 
			return rs->getCapabilities()->getSupportedShaderProfiles();
        }

    //---------------------------------------------------------------------------
    bool GpuProgramManager::isSyntaxSupported(const String& syntaxCode) const
        {
			// Use the current render system
			RenderSystem* rs = CamelotEngine::RenderSystemManager::getActive();

			// Get the supported syntaxed from RenderSystemCapabilities 
			return rs->getCapabilities()->isShaderProfileSupported(syntaxCode);
		}

	//---------------------------------------------------------------------------
	String GpuProgramManager::gpuProgProfileToRSSpecificProfile(GpuProgramProfile gpuProgProfile) const
	{
		// Use the current render system
		RenderSystem* rs = CamelotEngine::RenderSystemManager::getActive();

		return  rs->getCapabilities()->gpuProgProfileToRSSpecificProfile(gpuProgProfile);
	}
	//-----------------------------------------------------------------------------
	GpuProgramParametersSharedPtr GpuProgramManager::createParameters(void)
	{
		return GpuProgramParametersSharedPtr(new GpuProgramParameters());
	}
	//---------------------------------------------------------------------
	GpuSharedParametersPtr GpuProgramManager::createSharedParameters(const String& name)
	{
		if (mSharedParametersMap.find(name) != mSharedParametersMap.end())
		{
			CM_EXCEPT(InvalidParametersException, 
				"The shared parameter set '" + name + "' already exists!");
		}
		GpuSharedParametersPtr ret(new GpuSharedParameters(name));
		mSharedParametersMap[name] = ret;
		return ret;
	}
	//---------------------------------------------------------------------
	GpuSharedParametersPtr GpuProgramManager::getSharedParameters(const String& name) const
	{
		SharedParametersMap::const_iterator i = mSharedParametersMap.find(name);
		if (i == mSharedParametersMap.end())
		{
			CM_EXCEPT(InvalidParametersException, 
				"No shared parameter set with name '" + name + "'!");
		}
		return i->second;
	}
	//---------------------------------------------------------------------
	const GpuProgramManager::SharedParametersMap& 
	GpuProgramManager::getAvailableSharedParameters() const
	{
		return mSharedParametersMap;
	}
	//---------------------------------------------------------------------

}
