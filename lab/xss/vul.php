<!DOCTYPE html>
<html lang="en"> <!----( html ) ------------------------------->
  <head> <!---[ ahead ]---------------------------------------->
    <title> vul server </title>
  </head> <!--[ ahead ]---------------------------------------->
  <body> <!---/ mbody /---------------------------------------->
      <!-- ################## ##################################
           ############[ Has Token? ]###########################
           ############y############n###########################
           ############ ############ ###########################
           ############ ############[ POST Request? ]###########
           ######[ Has _GET? ]######y###############n###########
           ######y###########n###### ############### ###########
           ###### ########### ###### ############### ###########
           ###### ########### ##[ Generate token ]## ###########
           [ Velcome Page ]## ###################### ###########
           ################## ###################### ###########
           ################## ###############[ Fill form ]######    
           #########[ Redirect to ?usr= ]#######################
           #################################################    
           #####################################################
           #################################################    
           #####################################################
           #################################################    
           #####################################################
         ------------------------------------------------------>
      <?php
      session_start();
      // make storage for tokens
      if (!isset($_SESSION['usr'])){
          $_SESSION['usr'] = [];
      }
      function generate_token() {
          return random_int(1, 100);
      }
      function redirect($usr) {
          header('Location: ' . $_SERVER['PHP_SELF']
               . '?usr=' . $usr);
          exit();
      }
      // Check if the request has a cookie
      if (isset($_COOKIE['token'])) {
          // Check for usr query
          if (isset($_GET['usr'])) {
              echo 'Velcome ' . $_GET['usr'] . '!'; // cross
              exit();
          } else {
              $usr = $_SESSION['usr'][$_COOKIE['token']];
              redirect($usr);
          }
      } else {
          // User has submited a form?
          if ($_SERVER['REQUEST_METHOD'] == 'POST') {
              $token = generate_token();
              $usr = $_POST['usr'];
              // save (token => usr) mapping
              $_SESSION['usr'][$token] = $usr;
              setcookie('token', $token);
              // redirect to velcome page!
              redirect($usr);
          } else { // NOPE
              echo '
              <form action="" method="POST">
                <label for="usrinput"> Enter your name: </label>
                <input type="text" id="usrinput" name="usr">
                <input type="submit" value="Submit">
              </form>';
          }
          //////////////////////////////////////////////////////
          // (1) 毎日、何時に起きますか。
          //     十二時、起きます。
          //////////////////////////////////////////////////////
          // (2) たいてい、何時間ぐらい寝ますか。
          //     八時間ぐらい、寝ます。
          //////////////////////////////////////////////////////
          // (3) 毎日、何時間勉強しますか。
          //     九時間ぐらい勉強します。
          //////////////////////////////////////////////////////
          // (4) よくだれと昼ご飯を食べますか。
          //     いいえ、ぜんぜんだれと食べません。
          //////////////////////////////////////////////////////
          // (5) よく友だちにメールをしますか。
          //     いいえ、ぜんぜんメールしません。
          //////////////////////////////////////////////////////
          // (6) 先週、スポーツをしましたか。
          //     いいえ、あまりしませんでした。
          //////////////////////////////////////////////////////
          // (7) きのう、どこで晩ご飯を食べましたか。
          //     家で食べました。
          //////////////////////////////////////////////////////
          // (8) 先週、写真をたくさん撮りましたか。
          //     いいえ、あまり撮りませんでした。
          //////////////////////////////////////////////////////
          // (9) きのうは何曜日でしたか。
          //     日曜日でした。
          //////////////////////////////////////////////////////
      }
      ?>
  </body> <!--/ mbody /---------------------------------------->
</html> <!--------( html )------------------------------------->
