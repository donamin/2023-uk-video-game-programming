using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotationMatrix : MonoBehaviour
{
    public float theta = 0;
    public GameObject rotatedObject;
    //TODO (Q2): Define a reference for the second game object
    //...

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

        float s = Mathf.Sin(theta * Mathf.Deg2Rad);
        float c = Mathf.Cos(theta * Mathf.Deg2Rad);
        float x = transform.position.x;
        float z = transform.position.z;

        rotatedObject.transform.position = new Vector3(
            x * c - z * s
            , 0
            , x * s + z * c
        );

        //TODO (Q2): Update the position of the second game object such that it rotates -theta degrees
        //...
    }
}