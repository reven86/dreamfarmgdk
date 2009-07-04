// texture
texture DiffuseTex;

technique TDefaultShader
{
    pass P0
    {
        // material
		AlphaBlendEnable	= true;
		SrcBlend			= DestColor;
		DestBlend			= Zero;

        // samplers
		Texture[0]			= <DiffuseTex>;
    }
}
