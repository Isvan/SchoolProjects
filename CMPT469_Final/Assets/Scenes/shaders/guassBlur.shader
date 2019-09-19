Shader "Custom/gaussBlur"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
		_KernelSize("Kernel Size",Float) = 1
		_Sigma("Sigma",Float) = 2

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
			float _Sigma;
			//fixed2 = textureSize(_MainTex,0);

			fixed4 frag (v2f i) : SV_Target
			{

				

				float halfSize = (_KernelSize-1)/2;
				float centerOffset = (_KernelSize+1)/2;
				float weight = 0;
				fixed4 col = fixed4(0.0,0.0,0.0,0.0);
				//This will do -1 0 1
				//Init all them here to save space
				float x1,y1,gaussVal = 0;
				for(float x = -halfSize;x < halfSize;x++){
					for(float y = -halfSize; y < halfSize;y++){
						
						//When are in the middle we want to sample the middle of the guass function, but also sample the proper pixal
						//Hence the offsets
						x1 = x + centerOffset;
						y1 = y + centerOffset;

						//The hard part this could be offloaded somewhere else as we are calculating the gauss values each pixal
						gaussVal = exp( -((x1-centerOffset)*(x1-centerOffset) + (y1-centerOffset)*(y1-centerOffset))  / (_Sigma));

						//Make sure its not 0
						gaussVal = max(0,gaussVal);

						col += tex2D(_MainTex,float2(i.uv[0] + _MainTex_TexelSize.x * x,i.uv[1] + _MainTex_TexelSize.y * y)) * gaussVal;
						weight += gaussVal;
					}
				}

				return col/weight;
			}
			ENDCG
		}
	}
}
