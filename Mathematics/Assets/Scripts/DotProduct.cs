using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DotProduct : MonoBehaviour
{
    public float sightAngle = 45;
    public float sightRange = 15;
    public GameObject enemy;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Debug.DrawLine(transform.position, transform.position + transform.forward * 25
            , Color.blue);

        Vector3 fwd = transform.forward;
        Vector3 e = enemy.transform.position - transform.position;

        // compute the alpha between vectors fwd & e
        float dot = fwd.x * e.x + fwd.y * e.y + fwd.z * e.z;
        //float dot = Vector3.Dot(fwd, e);
        float alpha = Mathf.Acos(dot / (fwd.magnitude * e.magnitude)) * Mathf.Rad2Deg;

        //TODO (Q1): Update alpha such that its value is in [-180, 180]
        //...

        Debug.Log(alpha);

        // compute the distance between me and enemy
        float dist = Vector3.Distance(transform.position, enemy.transform.position);
        //float dist = e.magnitude;
        //float dist = Vector3.Magnitude(e);

        // draw a red line if the enemy is close enough and within sight, otherwise draw a green line.
        if (alpha < sightAngle && dist < sightRange)
            Debug.DrawLine(transform.position, enemy.transform.position, Color.red);
        else
            Debug.DrawLine(transform.position, enemy.transform.position, Color.green);

        // compute the projection of enemy's position vector onto my right vector
        Vector3 projDir = transform.right;
        float projNorm = Vector3.Dot(e, transform.right);// / transform.right.magnitude;
        Debug.DrawLine(transform.position, transform.position + projNorm * projDir, Color.blue);
    }
}