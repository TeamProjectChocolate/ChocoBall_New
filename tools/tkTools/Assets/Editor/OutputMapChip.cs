using UnityEngine;
using System.Collections;
using System.Text;
using System;

public class OutputMapChip : MonoBehaviour
{

    public static void ShowWindow(string mystring4)
    {
        GameObject MapChip = GameObject.Find("MapChip");
        string headerTxt = "";

        if (MapChip == null)
        {
            Debug.Log("親ないよ。\n");
            // そもそも一番上の親がない場合。
            headerTxt += string.Format("// データがありません。\n");
            headerTxt += "{\n";
            headerTxt += string.Format("\t\"NotData\",             //ファイル名\n");
            headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f),             //平行移動\n");
            headerTxt += string.Format("\tD3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f ),    //回転\n");
            headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f ),            //拡大\n");
            headerTxt += string.Format("\t999,    //屈折率タイプ\n");
            headerTxt += string.Format("\t1.0f,      //透明度");
            headerTxt += "},\n";
        }
        else
        {
            Transform[] Child = MapChip.GetComponentsInChildren<Transform>();

            if (Child.Length == 1)
            {
                // 一番上の親しかない場合。
                Debug.Log("親しかないよ。\n");
                headerTxt += string.Format("// データがありません。\n");
                headerTxt += "{\n";
                headerTxt += string.Format("\t\"NotData\",             //ファイル名\n");
                headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f),             //平行移動\n");
                headerTxt += string.Format("\tD3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f ),    //回転\n");
                headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f ),            //拡大\n");
                headerTxt += string.Format("\t999,    //屈折率タイプ\n");
                headerTxt += string.Format("\t1.0f,      //透明度");
                headerTxt += "},\n";
            }
            else
            {
                foreach (Transform tr in Child)
                {
                    // 余分なものは出力しない。
                    if(tr.tag == "MapObject")
                    {
                        headerTxt += string.Format("//{0}\n", tr.name);
                        headerTxt += "{\n";
                        headerTxt += string.Format("\t\"{0}.x\",             //ファイル名\n", tr.name);
                        headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
                        headerTxt += string.Format("\tD3DXQUATERNION({0:f}f, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
                        headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
                        headerTxt += string.Format("\t{0},    //屈折率タイプ\n",(int)(tr.GetComponent<MapChip>().m_RType));
                        headerTxt += string.Format("\t{0:f}f,      //透明度\n", tr.GetComponent<MapChip>().m_Alpha);
                        headerTxt += "},\n";
                    }
                }
            }
        }
        System.IO.StreamWriter sw = new System.IO.StreamWriter("../../ChocoBall/MapChipOutputByUnity" + mystring4 + ".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }
}
