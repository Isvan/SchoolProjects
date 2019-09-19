Shader "Custom/bilateralEffectThree"
{
	Properties
	{
		_MainTex("Texture", 2D) = "white" {}
	_KernelSize("Kernel Size",Float) = 1
		_SigmaX("SigmaX",Float) = 2
		_SigmaY("SigmaY",Float) = 2


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

	v2f vert(appdata v)
	{
		v2f o;
		o.vertex = UnityObjectToClipPos(v.vertex);
		o.uv = v.uv;
		return o;
	}

	sampler2D _MainTex;
	float4 _MainTex_TexelSize;
	float _KernelSize;
	float _SigmaX;
	float _SigmaY;

	//fixed2 = textureSize(_MainTex,0);

	fixed4 frag(v2f i) : SV_Target
	{



	float halfSize = (_KernelSize - 1) / 2;

	//Init inner loop vars here for speed
	float weight = 0;
	half4 texVal;
	half4 texValCenter = tex2D(_MainTex, i.uv);
	float centerBrit = length(texValCenter.rgb);
	fixed4 col = fixed4(0.0,0.0,0.0,0.0);

	float gaussABot = -1.0 / (2.0*_SigmaX*_SigmaX);
	float gaussBBot = 1.0 / (2.0*_SigmaY*_SigmaY);

	float distATop = 0;
	float distBTop = 0;

	float weightA = 0;
	float weightB = 0;


	fixed4 val1 = fixed4(0,0,0,0);
	float val1W = 0;

	fixed4 val2 = fixed4(0,0,0,0);
	float val2W = 0;

	//This will do -1 0 1
	//Init all them here to save space
	float x1,y1,gaussVal = 0;
	for (float x = -halfSize; x < halfSize; x++) {
		for (float y = -halfSize; y < halfSize; y++) {
			
			texVal = tex2D(_MainTex, float2(i.uv[0] + _MainTex_TexelSize.x * x, i.uv[1] + _MainTex_TexelSize.y * y));
			
			distATop = length(float2(x, y));
			distBTop = length(texVal.rgb) - centerBrit;

			weightA = exp(gaussABot*distATop*distATop);
			weightB = exp(gaussBBot*distBTop*distBTop);

			gaussVal = weightA * weightB;

			val1 += texVal * weightA;
			val2 += texVal * weightB;
			
			val1W += weightA;
			val2W += weightB;
			
			col += texVal * gaussVal;
			weight += gaussVal;
		}
	}

	

	// val1 = (col/weight) - (val1/val2W);

	// // if(val1.r > 0){
	// // 	val1.rgb = 1;
	// // }
	// val1.r = length(val1.rgb);
	// val1.gb = val1.r;
	// val1 = 1 - val1;
	// //val1 = val1;

	// //Clap values
	// val1 = min(1,val1);
	// val1 = max(0,val1);

	

	return  col/weight;
	//return val1;
	}
		ENDCG
	}
	}
}
