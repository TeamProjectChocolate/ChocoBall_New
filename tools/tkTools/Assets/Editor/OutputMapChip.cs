using UnityEngine;
using System.Collections;
using System.Text;

public class OutputMapChip : MonoBehaviour
{

    public static void ShowWindow(string mystring4)
    {
        GameObject MapChip = GameObject.Find("MapChip");
        if(MapChip == null)
        {
            return;
        }
        Transform[] Child = MapChip.GetComponentsInChildren<Transform>();
        string headerTxt = "";
        foreach (Transform tr in Child)
        {
            if (tr == MapChip)
            {
                // 親は出力しない。
                continue;
            }
            else
            {
                headerTxt += string.Format("//{0}\n", tr.name);
                headerTxt += "{\n";
                headerTxt += string.Format("\t{0}.x,             //ファイル名\n", tr.name);
                headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
                headerTxt += string.Format("\tD3DXQUATERNION({0:f}, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
                headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
                headerTxt += "},\n";
            }
        }
        System.IO.StreamWriter sw = new System.IO.StreamWriter("../../ChocoBall/MapChipOutputByUnity" + mystring4 + ".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }
}
