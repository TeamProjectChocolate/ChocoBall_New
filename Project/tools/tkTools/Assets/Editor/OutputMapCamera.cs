using UnityEngine;
using System.Collections;
using System.IO;

public class OutputMapCamera : MonoBehaviour {

    public static void ShowWindow(string mystring5)
    {
        GameObject EM_Camera = GameObject.Find("EM_Camera");
        string headerTxt = "";

        if (EM_Camera)
        {
            headerTxt += string.Format("//環境マップ用カメラ\n");
            headerTxt += "{\n";
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),        //平行移動\n", EM_Camera.transform.position.x, EM_Camera.transform.position.y, EM_Camera.transform.position.z);
            headerTxt += "},\n";
        }
        else
        {
            // 環境マップカメラがエディター上にない。
            headerTxt += string.Format("//環境マップ用カメラ\n");
            headerTxt += "{\n";
            headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f),        //平行移動\n");
            headerTxt += "},\n";
        }
        StreamWriter sw = new StreamWriter("../../ChocoBall/OutputMapCamera" + mystring5 + ".h", false, System.Text.Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }
}
