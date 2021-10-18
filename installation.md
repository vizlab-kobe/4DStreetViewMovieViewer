◆◆ 4DStreetViewMovieViewerの設定手順 ◆◆（指示がない限りターミナルで行う）  
＜準備編＞
1. Gitを必要に応じてインストールする  
   `git --version`  
     → バージョン情報が表示されると、既にインストールされている  
     → 入っていない場合、Xcodeをインストールする
2. Homebrewを必要に応じてインストールする  
   `brew list`  
    → リストが表示されていれば既にインストールされている  
    　 下記コマンドを実行して、homebrewとgitの最新版のインストールを行う  
    　 `brew update`  
    　 `brew install git`  
    → 『-bash:brew:command not found』が返ってきた場合は、Homebrewをインストールする。  
    　 下記をターミナル上で実行する。  
    　 `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"`  
3.  OpenCVを必要に応じてインストールする  
   バージョン4でないとffmpegを使うことができない  
   `brew info opencv`  
   → ファイル情報や『Poured from bottle on 2020-03-23 at 09:56:53』などが表示されれば既にインストールされている。  
   → 『Not installed』などと表示されていればこれからインストールを行う。  
   　 `brew install opencv`
4.  Qtをインストール、設定を行う  
   4-1. Qtを必要に応じてインストール  
         `brew info qt`  
         → ファイル情報や『Poured from bottle on 2020-03-23 at 09:56:53』などが表示されれば既にインストールされている。 
         → 『Not installed』などと表示されていればこれからインストールを行う。  
               `brew install qt`  
   4-2. Qtの設定を行う  
         ファイル`/usr/local/Cellar/qt/5.14.0/mkspecs/common/macx.conf`をテキストエディタで開く  
         QMAKE_MACOSX_DEPLOYMENT_TARGETをMacOSのバージョンに設定する  

〈インストール手順〉
1. Homeディレクトリの下にGitHub用のディレクトリを作成する。  
　　`$mkdir GitHub`  
2. KVSのインストール先のディレクトリを作成する。  
　　※ 例として、Homeディレクトリの下にlocalディレクトリを作成し、その下にkvsディレクトリを作成する。  
　　2.1 localディレクトリの作成　`$mkdir local`  
　　2.2 localディレクトリへ移動　`$cd local`  
　　2.3 kvsディレクトリを作成　  `$mkdir kvs`  
　　2.4 Homeディレクトリへ戻る `$cd`  
3. 1で作成したGitHubディレクトリへ移動。  
　　`$cd GitHub`  
4. githubからKVSをcloneする。  
　　`$git clone https://github.com/naohisas/KVS.git`  
　　　→　GitHubディレクトリの下にKVSディレクトリが新しく作成され、その下にソースがcloneされる。  
5. 4で作成されたKVSディレクトリへ移動。  
　　`$cd KVS`  
6. kvs.confの内容を一部書き換える
　　
```
#=============================================================================                                  
#  KVS enable/support flags
#  $Id: kvs.conf 1525 2013-04-11 02:58:08Z naohisa.sakamoto@gmail.com $
#=============================================================================
KVS_ENABLE_OPENGL     = 1
KVS_ENABLE_GLU        = 1
KVS_ENABLE_GLEW       = 0
KVS_ENABLE_OPENMP     = 0
KVS_ENABLE_DEPRECATED = 0

KVS_SUPPORT_CUDA      = 0
KVS_SUPPORT_GLUT      = 1
KVS_SUPPORT_OPENCV    = 1
KVS_SUPPORT_QT        = 1
KVS_SUPPORT_PYTHON    = 0
KVS_SUPPORT_EGL       = 0
KVS_SUPPORT_OSMESA    = 0
```
7. 
