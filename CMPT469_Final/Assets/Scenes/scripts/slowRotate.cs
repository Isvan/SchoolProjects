using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class slowRotate : MonoBehaviour {

    public float rotationSpeed = 1.0f;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {

        transform.Rotate(0, Time.deltaTime * rotationSpeed, 0);

    }
}
