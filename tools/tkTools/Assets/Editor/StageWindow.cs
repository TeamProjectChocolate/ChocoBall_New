using UnityEngine;
using UnityEditor;
using System.Collections;

class StageWindow : EditorWindow
{
    string myString = "";
    string myString1 = "";
    string myString2 = "";
    string myString3 = "";


    [MenuItem("Window/ヘッダー出力")]
    public static void ShowWindow()
    {
        EditorWindow.GetWindow(typeof(StageWindow));
    }

    void OnGUI()
    {
        // The actual window code goes here
        GUILayout.Label("Stage Settings", EditorStyles.boldLabel);

        //Windowのコリジョンの表示
        myString = EditorGUILayout.TextField("コリジョン", myString);
        //コリジョンのボタンが押されtureが返って来る。
        if (GUILayout.Button("コリジョン 出力"))
        {

            //空白の時の例外処理
            if (myString == "")
            {
                Debug.Log("文字列を入力してください。");
            }
            else
            {
                //コリジョン出力の処理
                OutputCollitionInfo.ShowWindow(myString);
                Debug.Log("コリジョンが無事出力できました。");
            }
        }

        //Windowのコース定義の表示
        myString1 = EditorGUILayout.TextField("コース定義", myString1);
        //コース定義のボタンが押されtureが返って来る。
        if (GUILayout.Button("コース定義 出力"))
        {
            //空白の時の例外処理
            if (myString1 == "")
            {
                Debug.Log("文字列を入力してください。");
            }
            else
            {
                //コース定義出力の処理
                OutputCourseDef.ShowWindow(myString1);
                Debug.Log("コース定義が無事出力できました。");
            }
        }

        //Windowの敵とギミックの情報の表示
        myString2 = EditorGUILayout.TextField("敵とギミックの情報", myString2);
        //敵とギミックの情報のボタンが押されtureが返って来る。
        if (GUILayout.Button("敵とギミックの情報を出力"))
        {
            //空白の時の例外処理
            if (myString2 == "")
            {
                Debug.Log("文字列を入力してください。");
            }
            else
            {
                //敵とギミックの情報の出力の処理
                OutputEnemyGimmick.ShowWindow(myString2);
                Debug.Log("敵とギミックの情報が無事出力できました。");
            }
        }

        //Windowのプレイヤーの情報の表示
        myString3 = EditorGUILayout.TextField("プレイヤーの情報", myString3);
        //敵とギミックの情報のボタンが押されtureが返って来る。
        if (GUILayout.Button("プレイヤーの情報を出力"))
        {
            //空白の時の例外処理
            if (myString3 == "")
            {
                Debug.Log("文字列を入力してください。");
            }
            else
            {
                //敵とギミックの情報の出力の処理
                PlayerPositionInfo.ShowWindow(myString3);
                Debug.Log("プレイヤーの情報が無事出力できました。");
            }
        }
    }
}
