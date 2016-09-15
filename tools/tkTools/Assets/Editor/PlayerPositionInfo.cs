using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;
using System.Text;

public class PlayerPositionInfo : MonoBehaviour
{

    public static void ShowWindow(string mystring3)
    {
        GameObject Player = GameObject.Find("Player");
        Transform[] Players = Player.GetComponents<Transform>();
        string headerTxt = "";
        foreach (Transform tr in Players)
        {
            if (tr.gameObject == Player)
            headerTxt += string.Format("//{0}\n", tr.name);
            headerTxt += "{\n";
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
            headerTxt += string.Format("\tD3DXQUATERNION({0:f}, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
            headerTxt += "},\n";
        }

        StreamWriter sw = new StreamWriter("C:/Github/GameChocoBall/ChocoBall/PlayerPositionInfo"+mystring3+".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }
}