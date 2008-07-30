// texture
texture DiffuseTex;

technique TDefaultShader
{
    pass P0
    {
        // material
		AlphaBlendEnable	= true;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;

        // samplers
		Texture[0]			= <DiffuseTex>;        
    }
}
