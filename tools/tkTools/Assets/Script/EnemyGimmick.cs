using UnityEngine;
using System.Collections;


public class EnemyGimmick : MonoBehaviour {
    public enum ENEMY_TYPE{ LR = 0, FB, Jamp, Bullet, Boss, None = 999};
    public enum GIMMICK_TYPE{ Chocoball = 0, Wall, FallFloor, MoveFloor, UpFloor, FireJet, SmoekJet, None = 999};
    public ENEMY_TYPE enemyType = ENEMY_TYPE.None;
    public GIMMICK_TYPE gimmickType = GIMMICK_TYPE.None;
    public float MaxMove = 0;
    // Use this for initialization
    void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	}
   
}
