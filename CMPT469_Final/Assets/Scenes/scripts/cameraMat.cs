using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class cameraMat : MonoBehaviour {

	public Material mat1,mat2,mat3,mat4,mat5,mat6,mat7,mat8,mat9;
	Material active;
	public int lastDown;
	int major;
	int minor;

	public Text ShaderName;
	public Text Sigmas;


	// Use this for initialization
	void Start () {
		lastDown = 1;
			major = 0;
			minor = 1;
			ShaderName.text = "Normal";
			Sigmas.text = "";
			active = mat1;
	}
	
	// Update is called once per frame
	void Update () {
		
		if(Input.GetKeyDown(KeyCode.Keypad1)){
			active = mat1;
			ShaderName.text = "Normal";
			Sigmas.text = "";
		}
		
		if(Input.GetKeyDown(KeyCode.Keypad2)){
			active = mat2;
			ShaderName.text = "Laplacian";
			Sigmas.text = "";
		}
		
		if(Input.GetKeyDown(KeyCode.Keypad3)){
			active = mat3;
			ShaderName.text = "Normal + Laplacian";
			Sigmas.text = "";
		}

		if(Input.GetKeyDown(KeyCode.Keypad4)){
			active = mat4;
			ShaderName.text = "Box Blur";
			Sigmas.text = "";
		}

		if(Input.GetKeyDown(KeyCode.Keypad5)){
			active = mat5;
			ShaderName.text = "Gaussain Blur";
			Sigmas.text = "";
		}

		if(Input.GetKeyDown(KeyCode.Keypad6)){
			active = mat6;
			ShaderName.text = "Bilateral Blur";
			Sigmas.text = "Kernel Size : " + mat6.GetFloat("_KernelSize") + " SigmaX : " + mat6.GetFloat("_SigmaX") + " SigmaY : " + mat6.GetFloat("_SigmaY");
		}

		if(Input.GetKeyDown(KeyCode.Keypad7)){
			active = mat7;
			ShaderName.text = "Bilateral 'Cartoon' Style";
			Sigmas.text = "Kernel Size : " + mat7.GetFloat("_KernelSize") + " SigmaX : " + mat7.GetFloat("_SigmaX") + " SigmaY : " + mat7.GetFloat("_SigmaY");
		
		}

		if(Input.GetKeyDown(KeyCode.Keypad8)){
			active = mat8;
			ShaderName.text = "Bilateral Edge Selection";
			Sigmas.text = "Kernel Size : " + mat8.GetFloat("_KernelSize") + " SigmaX : " + mat8.GetFloat("_SigmaX") + " SigmaY : " + mat8.GetFloat("_SigmaY");
		
		}

		if(Input.GetKeyDown(KeyCode.Keypad9)){
			ShaderName.text = "Bilateral Anti-Edge Selection";
			active = mat9;
			Sigmas.text = "Kernel Size : " + mat9.GetFloat("_KernelSize") + " SigmaX : " + mat9.GetFloat("_SigmaX") + " SigmaY : " + mat9.GetFloat("_SigmaY");
		
		}

		if(Input.GetKeyDown(KeyCode.KeypadPlus)){
			major++;
		}

		
		if(Input.GetKeyDown(KeyCode.KeypadMinus)){
			major--;
		}

        if (Input.GetKey(KeyCode.Q))
        {
            active.SetFloat("_SigmaY", active.GetFloat("_SigmaY") - 0.001f);
			Sigmas.text = "Kernel Size : " + active.GetFloat("_KernelSize") + " SigmaX : " + active.GetFloat("_SigmaX") + " SigmaY : " + active.GetFloat("_SigmaY");
		
        }

        if (Input.GetKey(KeyCode.E))
        {
            active.SetFloat("_SigmaY", active.GetFloat("_SigmaY") + 0.001f);
			Sigmas.text = "Kernel Size : " + active.GetFloat("_KernelSize") + " SigmaX : " + active.GetFloat("_SigmaX") + " SigmaY : " + active.GetFloat("_SigmaY");
		
		}

		  if (Input.GetKey(KeyCode.R))
        {
            active.SetFloat("_SigmaX", active.GetFloat("_SigmaX") + 0.1f);
			Sigmas.text = "Kernel Size : " + active.GetFloat("_KernelSize") + " SigmaX : " + active.GetFloat("_SigmaX") + " SigmaY : " + active.GetFloat("_SigmaY");
		
        }

        if (Input.GetKey(KeyCode.F))
        {
            active.SetFloat("_SigmaX", active.GetFloat("_SigmaX") - 0.1f);
			Sigmas.text = "Kernel Size : " + active.GetFloat("_KernelSize") + " SigmaX : " + active.GetFloat("_SigmaX") + " SigmaY : " + active.GetFloat("_SigmaY");
		
		}


    }

    void OnRenderImage(RenderTexture src, RenderTexture dest) {
    	
		Graphics.Blit(src, dest, active);
	}
}
