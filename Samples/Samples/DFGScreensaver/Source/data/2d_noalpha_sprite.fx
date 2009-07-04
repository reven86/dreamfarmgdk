// texture
texture DiffuseTex;

technique TDefaultShader
{
    pass P0
    {
        // material
		AlphaBlendEnable	= false;
	
        // samplers
		Texture[0]			= <DiffuseTex>;
    }
}
