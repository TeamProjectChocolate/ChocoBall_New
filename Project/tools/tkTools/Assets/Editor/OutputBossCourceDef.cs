using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System.Text;

public class OutputBossCourceDef : MonoBehaviour {

    //[MenuItem("Window/コース定義 出力")]
    public static void ShowWindow(string mystring1)
    {
        GameObject cource = GameObject.Find("BossCource");
        Transform[] Cource = cource.GetComponentsInChildren<Transform>();
        List<GameObject> CourceDefList = new List<GameObject>();
        foreach (Transform tr in Cource)
        {
            if (tr.gameObject.tag == "BossCource")
            {
                // コース定義の塊を持つ親のみを取得。
                CourceDefList.Add(tr.gameObject);
            }
        }

        string headerTxt = "";
        foreach (var CourceDef in CourceDefList)
        {
            Vector3 Start, End, Range,RangePos,ActionPos,ActionPosEnd,ActionDir;
            Start = End = Range = RangePos = ActionPos = ActionPosEnd = ActionDir = new Vector3(0.0f, 0.0f, 0.0f);
            Quaternion Rotation = new Quaternion();
            Transform[] Childs = CourceDef.GetComponentsInChildren<Transform>();
            foreach(var Child in Childs)
            {
                switch (Child.tag)
                {
                    case "CourceStart":
                        Start = Child.transform.position;
                        break;
                    case "CourceEnd":
                        End = Child.transform.position;
                        break;
                    case "CourceRange":
                        Range = Child.transform.localScale;
                        RangePos = Child.transform.position;
                        Rotation = Child.transform.rotation;
                        break;
                    case "CourceActionPos":
                        ActionPos = Child.transform.position;
                        break;
                    case "CourceActionPosEnd":
                        ActionPosEnd = Child.transform.position;
                        break;
                }
            }
            ActionDir = ActionPosEnd - ActionPos;
            ActionDir.Normalize();

            headerTxt += string.Format("//{0}\n", CourceDef.name);
            headerTxt += "{\n";
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f), //Start\n", Start.x, Start.y, Start.z);
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f), //End\n", End.x, End.y, End.z);
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f), //Range\n", Range.x, Range.y, Range.z);
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f), //RangePos\n", RangePos.x, RangePos.y, RangePos.z);
            headerTxt += string.Format("\tD3DXQUATERNION({0:f}f, {1:f}f, {2:f}f, {3:f}f), //Rotation\n", Rotation.x, Rotation.y, Rotation.z, Rotation.w);
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f), //ActionTargetDir\n", ActionDir.x, ActionDir.y, ActionDir.z);
            headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f), //ActionPos\n", ActionPos.x, ActionPos.y, ActionPos.z);
            headerTxt += string.Format("\t{0}, //CourceType\n",(int)CourceDef.gameObject.GetComponent<BossCource>().Type);
            if(CourceDef.gameObject.GetComponent<BossCource>().IsEnd){
                headerTxt += string.Format("\ttrue, //IsEnd\n");
            }
            else
            {
                headerTxt += string.Format("\tfalse, //IsEnd\n");
            }
            headerTxt += "},\n";
        }

        StreamWriter sw = new StreamWriter("../../ChocoBall/BossCourceDef" + mystring1 + ".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }
}
