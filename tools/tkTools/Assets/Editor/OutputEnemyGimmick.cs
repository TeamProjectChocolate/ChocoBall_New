using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;
using System.Text;

public class OutputEnemyGimmick : MonoBehaviour {
    //[MenuItem("Window/敵とギミックの情報を出力")]
    public static void ShowWindow(string mystring2)
    {
        GameObject go = GameObject.Find("EnemyGimmick");//エネミーと敵のオブジェクトを見つける
        Transform[] children = go.GetComponentsInChildren<Transform>();//EnemyGimmickの子を探してくる
        //まずギミックトリガーを先に出力する。
        string headerTxt = "";
        foreach (Transform tr in children)
        {
            if(tr.gameObject == go) //親はいらない
            {
                continue;
            }
            if(tr.name == "GimmickTrigger")
            {
                headerTxt += string.Format("//{0}\n", tr.name);
                headerTxt += "{\n";
                headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
                headerTxt += string.Format("\tD3DXQUATERNION({0:f}, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
                headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
                headerTxt += "},\n";
            }
        }
        StreamWriter sw = new StreamWriter("C:/Github/GameChocoBall/ChocoBall/GimmickTriggerInfo"+mystring2+".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
        headerTxt = "";
        foreach (Transform tr in children)
        {
            if (tr.gameObject == go)//親はいらない
            {
                continue;
            }
            if (tr.gameObject.name != "GimmickTrigger")
            {
                headerTxt += string.Format("//{0}\n", tr.name);
                headerTxt += "{\n";
                headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
                headerTxt += string.Format("\tD3DXQUATERNION({0:f}, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
                headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
                headerTxt += string.Format("\t{0},//エネミータイプ\n", tr.GetComponent<EnemyGimmick>().enemyType);
                headerTxt += string.Format("\t{0},//ギミックタイプ\n", tr.GetComponent<EnemyGimmick>().gimmickType);
                headerTxt += string.Format("\t{0:f}f,//最大移動量\n", tr.GetComponent<EnemyGimmick>().MaxMove);
                headerTxt += "},\n";
            }
           
        }

        sw = new StreamWriter("C:/Github/GameChocoBall/ChocoBall/EnemyGimmickInfo"+mystring2+".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
    }
}
