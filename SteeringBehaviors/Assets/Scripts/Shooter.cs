using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Shooter : MonoBehaviour
{
    public LayerMask targetLayers;
    public GameObject target, patrolTarget;

    public int nRays = 25;

    public bool hasUnlimitedSight = false, shootAtOldTargetLocation = false;
    public float sightRange = 25, sightAngle = 45;

    public GameObject bulletPrefab;
    public int shootingFrequency = 5;
    public float bulletSpeed = 25, bulletLifetime = 2, aimError = 10;

    public float patrolRadius = 5, patrolRotationSpeed = 10;
    float patrolTargetAngle = 0;

    float lastShootTime = 0;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (target && bulletPrefab && Time.time - lastShootTime >= 1.0f / shootingFrequency)
        {
            RaycastHit hit = new RaycastHit();
            bool shot = false, spottedEnemy = false;
            for (int i = 0; i < nRays; i++)
            {
                Vector3 dir = Vector3.zero;
                if (hasUnlimitedSight)
                {
                    float theta = 360.0f / nRays * i * Mathf.Deg2Rad;
                    dir = new Vector3(Mathf.Sin(theta), 0, Mathf.Cos(theta));
                }
                else
                {
                    float theta = ((float)(i) / (nRays - 1) * 2 - 1) * sightAngle;
                    dir = VectorRotateY(transform.forward, theta);
                }

                //Debug.DrawLine(transform.position, transform.position + sightRange * dir, Color.green);

                if(Physics.Raycast(transform.position, dir, out hit, sightRange, targetLayers))
                {
                    spottedEnemy = true;
                    GetComponent<SeekAndFlee>().target = hit.collider.gameObject;
                    Debug.DrawLine(transform.position, hit.point, Color.red);
                    if (!shot)
                    {
                        Vector3 aim = Vector3.zero;
                        if (shootAtOldTargetLocation)
                        {
                            aim = (hit.collider.gameObject.transform.position - transform.position).normalized;
                        }
                        else
                        {
                            float t_b = (hit.collider.gameObject.transform.position - transform.position).magnitude / bulletSpeed;
                            Vector3 p2 = hit.collider.gameObject.transform.position + t_b * hit.collider.gameObject.GetComponent<Kinematics>().velocity;
                            //Debug.DrawLine(transform.position, p2, Color.white);
                            aim = (p2 - transform.position).normalized;
                        }
                        aim = VectorRotateY(aim, Random.Range(-aimError, aimError));
                        Shoot(aim);
                        shot = true;
                    }
                }
                else
                {
                    Debug.DrawLine(transform.position, transform.position + sightRange * dir, Color.green);
                }
            }
            if(!spottedEnemy && patrolTarget)
            {
                GetComponent<SeekAndFlee>().target = patrolTarget;
                patrolTargetAngle += patrolRotationSpeed * (2 * Random.Range(0, 2) - 1) * Time.deltaTime;
                patrolTargetAngle = Mathf.Clamp(patrolTargetAngle, -45, 45);
                patrolTarget.transform.position = transform.position + VectorRotateY(transform.forward, patrolTargetAngle) * patrolRadius;
            }
        }
    }

    void Shoot(Vector3 dir)
    {
        GameObject bullet = Instantiate(bulletPrefab, transform.position, Quaternion.identity);
        bullet.GetComponent<Rigidbody>().velocity = dir * bulletSpeed;
        Physics.IgnoreCollision(bullet.GetComponent<Collider>(), GetComponent<Collider>());
        Destroy(bullet, bulletLifetime);
        lastShootTime = Time.time;
    }

    Vector3 VectorRotateY(Vector3 v, float theta)
    {
        theta *= Mathf.Deg2Rad;

        float sin = Mathf.Sin(theta);
        float cos = Mathf.Cos(theta);

        float newX = v.x * cos - v.z * sin;
        float newZ = v.x * sin + v.z * cos;

        return new Vector3(newX, 0, newZ);
    }
}