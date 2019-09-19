Shader "Custom/edgeMorph"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
	}
	SubShader
	{
		// No culling or depth
		Cull Off ZWrite Off ZTest Always

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float2 uv : TEXCOORD0;
			};

			struct v2f
			{
				float2 uv : TEXCOORD0;
				float4 vertex : SV_POSITION;
			};

			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				return o;
			}
			
			sampler2D _MainTex;
			float4 _MainTex_TexelSize;

			fixed4 frag (v2f i) : SV_Target
			{
				
				float size = 5;
				fixed4 col = fixed4(0.0,0.0,0.0,0.0);
				col += tex2D(_MainTex,float2(i.uv[0] + 1.0f * _MainTex_TexelSize.x * size,i.uv[1]));
				col += tex2D(_MainTex,float2(i.uv[0] - 1.0f * _MainTex_TexelSize.x  * size,i.uv[1]));
				col += tex2D(_MainTex,float2(i.uv[0],i.uv[1] - 1.0f * _MainTex_TexelSize.y  * size));
				col += tex2D(_MainTex,float2(i.uv[0],i.uv[1] + 1.0f * _MainTex_TexelSize.y  * size));			
				

				col += tex2D(_MainTex,i.uv) * -4.0f;

				col.r = col.r * 0.3 + col.g * 0.4 + col.b * 0.3;
				col.gb = col.r;
				
				col.rbg = 1 - col.rbg;

				col.rgb = col.rgb * tex2D(_MainTex,i.uv);
				return max(0,col);
			}
			ENDCG
		}
	}
}
