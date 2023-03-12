using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trigonometry : MonoBehaviour
{
    public float scale = 0, speed = 0;

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = new Vector3(
            scale * Mathf.Sin(speed * Time.time)
            , 0
            , scale * Mathf.Cos(2 * speed * Time.time)
        );
    }
}