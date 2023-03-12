using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Norm : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 v = gameObject.transform.position;
        //float norm = v.magnitude;
        float norm = Mathf.Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        //Debug.Log(norm);

        /*if(norm < 5)
            Debug.DrawLine(Vector3.zero, v, Color.red);
        else
            Debug.DrawLine(Vector3.zero, v, Color.green);*/

        float normSquared = v.x * v.x + v.y * v.y + v.z * v.z;

        if (normSquared < 25)
            Debug.DrawLine(Vector3.zero, v, Color.red);
        else
            Debug.DrawLine(Vector3.zero, v, Color.green);

        Vector3 unitV = norm > 0 ? v / norm : Vector3.zero;
        Debug.DrawLine(Vector3.zero, unitV, Color.blue);
    }
}