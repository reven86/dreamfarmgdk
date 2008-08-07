// texture
texture DiffuseTex;

technique TDefaultShader
{
    pass P0
    {
        // material
		ZEnable			= false;
		ZWriteEnable		= false;
		AlphaBlendEnable	= true;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;

        // samplers
		Texture[0]			= <DiffuseTex>;        
    }
}
