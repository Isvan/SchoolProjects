
Shader "Custom/lapacian"
{
    Properties
    {
        // we have removed support for texture tiling/offset,
        // so make them not be displayed in material inspector
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {

        // No culling or depth
		Cull Off ZWrite Off ZTest Always

        Pass
        {
            CGPROGRAM
            // use "vert" function as the vertex shader
            #pragma vertex vert
            // use "frag" function as the pixel (fragment) shader
            #pragma fragment frag

            // vertex shader inputs
            struct appdata
            {
                float4 vertex : POSITION; // vertex position
                float2 uv : TEXCOORD0; // texture coordinate
            };

            // vertex shader outputs ("vertex to fragment")
            struct v2f
            {
                float2 uv : TEXCOORD0; // texture coordinate
                float4 vertex : SV_POSITION; // clip space position
            };

            // vertex shader
            v2f vert (appdata v)
            {
                v2f o;
                // transform position to clip space
                // (multiply with model*view*projection matrix)
                o.vertex = UnityObjectToClipPos(v.vertex);
                // just pass the texture coordinate
                o.uv = v.uv;
                return o;
            }
            
            // texture we will sample
            sampler2D _MainTex;
			float4 _MainTex_TexelSize;

            // pixel shader; returns low precision ("fixed4" type)
            // color ("SV_Target" semantic)
            fixed4 frag (v2f i) : SV_Target
            {
				


                // sample texture and return it
				fixed4 col = fixed4(0.0,0.0,0.0,0.0);
				col += tex2D(_MainTex,float2(i.uv[0] + 1.0f * _MainTex_TexelSize.x,i.uv[1]));
				col += tex2D(_MainTex,float2(i.uv[0] - 1.0f * _MainTex_TexelSize.x,i.uv[1]));
				col += tex2D(_MainTex,float2(i.uv[0],i.uv[1] - 1.0f * _MainTex_TexelSize.y));
				col += tex2D(_MainTex,float2(i.uv[0],i.uv[1] + 1.0f * _MainTex_TexelSize.y));			
				col += tex2D(_MainTex,i.uv) * -4.0f;

				col.r = (col.r + col.g + col.b) / 3;
				col.gb = col.r;
				
				col.rbg = 1 - col.rbg;

				//col += tex2D(_MainTex,i.uv);

				return col;
            }
            ENDCG
        }
    }
}