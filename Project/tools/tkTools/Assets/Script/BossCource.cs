using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossCource : MonoBehaviour {
    public enum CourceType { Wait = 0,Move,Attack ,Sleep};

    [Tooltip("コースの属性(これでボスの行動を決定する)")]
    [SerializeField]
    private CourceType m_Type = CourceType.Wait;
    public CourceType Type
    {
        get { return m_Type; }
    }
    [Tooltip("最後のコースか")]
    [SerializeField]
    private bool m_IsEnd = false;
    public bool IsEnd
    {
        get { return m_IsEnd; }
    }

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
