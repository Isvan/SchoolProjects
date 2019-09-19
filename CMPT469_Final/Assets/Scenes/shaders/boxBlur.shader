Shader "Custom/boxBlur"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_KernelSize("Kernel Size",Float) = 1
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
			float _KernelSize;

			fixed4 frag (v2f i) : SV_Target
			{
				fixed4 col = fixed4(0,0,0,0);
				//fixed4 col = tex2D(_MainTex, i.uv);
				// just invert the colors
			
				float weight = 0;
				float halfSize = (_KernelSize+1)/2;

				for(int x = -halfSize;x < halfSize;x++){
					for(int y = -halfSize;y < halfSize;y++){

						col += tex2D(_MainTex,float2(i.uv[0] + _MainTex_TexelSize.x * x,i.uv[1] + _MainTex_TexelSize.y * y));

						weight += 1;
					}

				}

				return col / weight;
			}
			ENDCG
		}
	}
}
