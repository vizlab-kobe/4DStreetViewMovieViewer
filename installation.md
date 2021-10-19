◆◆ 4DStreetViewMovieViewerの設定手順 ◆◆（指示がない限りターミナルで行う）  
＜準備編＞
1. Gitを必要に応じてインストールする  
   `$git --version`  
     → バージョン情報が表示されると、既にインストールされている  
     → 入っていない場合、Xcodeをインストールする
2. Homebrewを必要に応じてインストールする  
   `$brew list`  
    → リストが表示されていれば既にインストールされている  
    　 下記コマンドを実行して、homebrewとgitの最新版のインストールを行う  
    　 `$brew update`  
    　 `$brew install git`  
    → 『-bash:brew:command not found』が返ってきた場合は、Homebrewをインストールする。  
    　 下記をターミナル上で実行する。  
    　 `$/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"`  
3.  OpenCVのversion4がインストールされていない場合、インストールする  
   バージョン4でないとffmpegを使うことができない  
   `$brew info opencv`  
   → 『opencv: stable 4.5.3 (bottled) 』などと表示されれば既にversion4がインストールされている。  
   → それ以外であれば以下でインストールを行う。  
   　 `$brew install opencv`
4.  Qtをインストール、設定を行う  
   4-1. Qtを必要に応じてインストール  
         `$brew info qt`  
         → ファイル情報や『Poured from bottle on 2020-03-23 at 09:56:53』などが表示されれば既にインストールされている。 
         → 『Not installed』などと表示されていればこれからインストールを行う。  
               `$brew install qt`  
   4-2. Qtの設定を行う  
         ファイル`$/usr/local/Cellar/qt/5.14.0/mkspecs/common/macx.conf`をテキストエディタで開く  
         QMAKE_MACOSX_DEPLOYMENT_TARGETをMacOSのバージョンに設定する  

〈4DStreetViewerのインストール手順〉
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
7. QtおよびKVSの環境変数を~/.zshrcに設定する  
```
export KVS_DIR=~/local/kvs
export PATH=$PATH:$KVS_DIR/bin                                                                                  
export KVS_CPP=g++
export PKG_CONFIG_PATH=/usr/local/opt/opencv@3/lib/pkgconfig:$PKG_CONFIG_PATH

export QMAKESPEC=macx-clang
export QTDIR=/usr/local/opt/qt
export PATH=$QTDIR/bin:$PATH
export DYLD_LIBRARY_PATH=$QTDIR/lib:$DYLD_LIBRARY_PATH
export PATH=/usr/local/sbin:$PATH
```
8. 7で設定した環境変数を反映させる  
　　`$source ~/.zshrc`  
9. KVSをコンパイル・インストールする  
　　9-1. KVSをコンパイル　`$make`  
　　9-2. KVSをインストール　`$make install`  
　　　　→ 2.で設定したloca/KVSにインストールされる  
10.  1で作成したGitHubディレクトリへ移動し、4DStreetViewMovieViewerをgithubからcloneする。  
　　10.1 GitHubディレクトリへ移動　　　　　　`$cd ../`　もしくは　`$cd ~/GitHub`  
　　10.2 4DStreetViewMovieViewerをcloneする　`$git clone https://github.com/keikoootsuji/4DStreetViewMovieViewer.git`  
　　　　　→　GitHubディレクトリの下に4DStreetViewMovieViewerディレクトリが新しく作成され、その下にソースがcloneされる。  
 11. 4DStreetViewMovieViewerをコンパイルする。  
　　11.1 Libのコンパイル  
　　　11.1-1 Libディレクトリへ移動　`$cd 4DStreetViewMovieViewer/Lib`  
　　　11.1-2 コンパイルする　　　 　`$./kvsmake.py`  
　　11.2 Viewerのコンパイル  
　　　11.2-1 App/Viewerへ移動　　`$cd ../App/Viewer`  
　　　11.2-2 proファイルの作成　　`$kvsmake -Q`  
　　　　　　　→ Viewer.proが作成される  
　　　11.2-3 Makefileの作成　　　   `$qmake`  
　　　　　　　→ Makefileが作成される  
　　　11.2-4 コンパイルする　　  　`$make`  
　　　　　　　→  Viewer.appが作成される  
       
〈実行手順〉  
1. confファイル(.4dsv)を作成/編集する  
　　Viewerでデータを表示するためのconfファイル ( .4dsv ) を作成または編集する。  
　　書き方は以下の通り。  
　　　1行目：データの場所  
　　　2行目：データの拡張子  
　　　3行目：データのカメラ配置（x、y、 z）　※ 半角スペースで区切る  
　　　4行目：読み込み後に表示させたいカメラ位置座標（x、y、 z）　※ 半角スペースで区切る  
　　　5行目：データの総フレーム数  
　　　6行目：データの情報  
　　以下の例を参考にしてください。  
  ```
  ~/local2/Data/4DStreetViewData/movies/                                                                          
mp4
4 4 4
2 1 0
15
このデータは、それぞれｘ方向４、ｙ方向４、z方向４のカメラ視点を持つデータです。
  ```
2. Viewerを起動する  
　　`$./run.sh`  
3. メニューバーのFILEボタンから適用する.4dsvファイルを選択する
