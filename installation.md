◆◆ 4DStreetViewMovieViewerの設定手順 ◆◆（全てターミナル上で行う。）  
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
5.  
