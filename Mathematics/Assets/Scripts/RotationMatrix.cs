using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotationMatrix : MonoBehaviour
{
    public float theta = 0;
    public GameObject rotatedObject;
    
    // Start is called before the first frame update
    void Start()
    {   
    }

    // Update is called once per frame
    void Update()
    {
        Debug.DrawLine(Vector3.zero, transform.position, Color.white);
        Debug.DrawLine(Vector3.zero, rotatedObject.transform.position
            , Color.yellow);
    }
}