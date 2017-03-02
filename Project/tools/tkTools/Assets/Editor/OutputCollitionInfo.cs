﻿using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;
using System.Text;

public class OutputCollitionInfo : EditorWindow
{

    //[MenuItem("Window/コリジョン 出力")]
    public static void ShowWindow(string mystring)
    {
		GameObject collision = GameObject.Find("Collision");
        Transform[] collisions = collision.GetComponentsInChildren<Transform>();
        string headerTxt = "";
        int GroundIdx = 0;
        int WallIdx = 0;
        foreach (Transform tr in collisions)
        {
            if(tr.gameObject == collision)
            {
                continue;
            }

            int idx = 0;
            switch(tr.tag){
                case "Ground":
                    idx = GroundIdx++;
                    break;
                case "Wall":
                    idx = WallIdx++;
                    break;
                default:
                    Debug.Log("タグをつけてください。");
                    return;
            }
            headerTxt += string.Format("//{0}\n", tr.tag + "_" + idx.ToString("00"));
            headerTxt += "{\n";
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
            headerTxt += string.Format("\tD3DXQUATERNION({0:f}f, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
            headerTxt += "},\n";
        }
        
        StreamWriter sw = new StreamWriter("../../ChocoBall/collisionInfo" + mystring+".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }

    void OnGUI()
    {
        
    }

}