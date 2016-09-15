using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;
using System.Text;

public class OutputCourseDef : MonoBehaviour {

    //[MenuItem("Window/コース定義 出力")]
    public static void ShowWindow(string mystring1)
    {
		GameObject cource = GameObject.Find("Cource");
        Transform[] courcePoint = cource.GetComponentsInChildren<Transform>();
        string headerTxt = "";
        foreach (Transform tr in courcePoint)
        {
            if (tr.gameObject == cource)
            {
                continue;
            }
            headerTxt += string.Format("D3DXVECTOR3({0:f}f, {1:f}f, {2:f}f), //{3}\n", tr.position.x, tr.position.y, tr.position.z, tr.name);
        }

        StreamWriter sw = new StreamWriter("C:/Github/GameChocoBall/ChocoBall/CourceDef"+mystring1+".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }

}
