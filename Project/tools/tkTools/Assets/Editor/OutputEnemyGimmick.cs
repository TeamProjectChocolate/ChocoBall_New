using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;
using System.Text;

public class OutputEnemyGimmick : MonoBehaviour {

    //[MenuItem("Window/敵とギミックの情報を出力")]
    public static void ShowWindow(string mystring2)
    {
        bool IsBreak = false;   // 終了フラグ。

        GameObject go = GameObject.Find("EnemyGimmick");//エネミーと敵のオブジェクトを見つける
        string headerTxt = "";

        if (go == null)
        {
            IsBreak = true;
        }
        else
        {
            Transform[] children = go.GetComponentsInChildren<Transform>();//EnemyGimmickの子を探してくる
            //まずギミックトリガーを先に出力する。
            foreach (Transform tr in children)
            {
                if (tr.gameObject == go) //親はいらない
                {
                    continue;
                }
                if (tr.tag == "GimmickTrigger")
                {
                    headerTxt += string.Format("//{0}\n", tr.name);
                    headerTxt += "{\n";
                    headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
                    headerTxt += string.Format("\tD3DXQUATERNION({0:f}f, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
                    headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
                    headerTxt += "},\n";
                }
            }

            if (headerTxt.Equals(""))
            {
                // ギミックトリガーがなかった場合。
                headerTxt += string.Format("//データがありません。\n");
                headerTxt += "{\n";
                headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f),             //平行移動\n");
                headerTxt += string.Format("\tD3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f ),    //回転\n");
                headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f ),            //拡大\n");
                headerTxt += "},\n";
            }
        }
        if (IsBreak)
        {
            headerTxt += string.Format("//データがありません。\n");
            headerTxt += "{\n";
            headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f),             //平行移動\n");
            headerTxt += string.Format("\tD3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f ),    //回転\n");
            headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f ),            //拡大\n");
            headerTxt += "},\n";
        }
        StreamWriter sw = new StreamWriter("../../ChocoBall/GimmickTriggerInfo"+mystring2+".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
        headerTxt = "";

        if (IsBreak)
        {
            headerTxt += string.Format("//データがありません\n");
            headerTxt += "{\n";
            headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f),             //平行移動\n");
            headerTxt += string.Format("\tD3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f ),    //回転\n");
            headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f ),            //拡大\n");
            headerTxt += string.Format("\t999,//エネミータイプ\n");
            headerTxt += string.Format("\t999,//ギミックタイプ\n");
            headerTxt += string.Format("\t0.0f,//最大移動量\n");
            headerTxt += "},\n";
        }
        else
        {
            Transform[] children = go.GetComponentsInChildren<Transform>();//EnemyGimmickの子を探してくる

            foreach (Transform tr in children)
            {
                if (tr.gameObject == go)//親はいらない
                {
                    continue;
                }
                if (tr.gameObject.tag != "GimmickTrigger")
                {
                    if(tr.GetComponent<EnemyGimmick>().enemyType != EnemyGimmick.ENEMY_TYPE.None && tr.GetComponent<EnemyGimmick>().gimmickType != EnemyGimmick.GIMMICK_TYPE.None)
                    {
                        Debug.Log("EnemyTypeとGimmickTypeのどちらにもNoneでないものが入力されています。\n");
                        headerTxt = "";
                        IsBreak = true;
                        break;
                    }
                    headerTxt += string.Format("//{0}\n", tr.name);
                    headerTxt += "{\n";
                    headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", tr.position.x, tr.position.y, tr.position.z);
                    headerTxt += string.Format("\tD3DXQUATERNION({0:f}f, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
                    headerTxt += string.Format("\tD3DXVECTOR3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
                    headerTxt += string.Format("\t{0},//エネミータイプ\n", (int)(tr.GetComponent<EnemyGimmick>().enemyType));
                    headerTxt += string.Format("\t{0},//ギミックタイプ\n", (int)(tr.GetComponent<EnemyGimmick>().gimmickType));
                    headerTxt += string.Format("\t{0:f}f,//最大移動量\n", tr.GetComponent<EnemyGimmick>().MaxMove);
                    headerTxt += "},\n";
                }
            }
            if (headerTxt.Equals(""))
            {
                // エネミーがいない場合。
                headerTxt += string.Format("//データがありません\n");
                headerTxt += "{\n";
                headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f),             //平行移動\n");
                headerTxt += string.Format("\tD3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f ),    //回転\n");
                headerTxt += string.Format("\tD3DXVECTOR3(0.0f, 0.0f, 0.0f ),            //拡大\n");
                headerTxt += string.Format("\t999,//エネミータイプ\n");
                headerTxt += string.Format("\t999,//ギミックタイプ\n");
                headerTxt += string.Format("\t0.0f,//最大移動量\n");
                headerTxt += "},\n";

                IsBreak = true;
            }
        }
        sw = new StreamWriter("../../ChocoBall/EnemyGimmickInfo" + mystring2+".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();
        if (IsBreak)
        {
            return;
        }
    }
}
