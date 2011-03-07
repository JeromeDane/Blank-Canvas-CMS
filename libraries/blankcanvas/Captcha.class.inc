<?php

namespace BlankCanvas;

require_once('libraries/recaptcha-php-1.11/recaptchalib.php');

class Captcha {
	protected static $_publickey = "6Ld108ESAAAAAD_-_tHrSCRMPHfaMBRVcMN-m6nf";
	protected static $_privatekey = "6Ld108ESAAAAAB-1hQJq7w32jgdFlkQ23RA5BQl1";
	public static function getHtml() {
		
		$pubkey = self::$_publickey;
        $server = RECAPTCHA_API_SECURE_SERVER;
        
        return
        '<style type="text/css">
        	#recaptcha_widget {  }
        	#recaptcha_image {
        		border:4px double;
        		width:245px !important;
        		background-color:#fff;
        		text-align:center;
        	}
        	#recaptcha_image img {
        		margin:0 !important;
        		width:245px;
        	}
        </style>
        <script type="text/javascript">
			 var RecaptchaOptions = {
			    theme : "custom",
			    custom_theme_widget: "recaptcha_widget"
			 };
		</script>
		
		<!--
		<script type="text/javascript" src="'. $server . '/challenge?k=' . $pubkey . '"></script>
		<noscript>
	  		<iframe src="'. $server . '/noscript?k=' . $pubkey . '" height="300" width="500" frameborder="0"></iframe><br/>
	  		<textarea name="recaptcha_challenge_field" rows="3" cols="40"></textarea>
	  		<input type="hidden" name="recaptcha_response_field" value="manual_challenge"/>
		</noscript>
		-->
		 <table id="recaptcha_widget" style="display:none;">
			<tr>
				<td id="recaptcha_image"></td>
				<td style="padding-left:.5em;">
				   <div><a href="javascript:Recaptcha.reload()"><img src="/libraries/blankcanvas/captcha//refresh.png"/></a></div>
				   <div class="recaptcha_only_if_image"><a href="javascript:Recaptcha.switch_type(\'audio\')"><img src="/libraries/blankcanvas/captcha//audio.png"/></a></div>
				   <div class="recaptcha_only_if_audio"><a href="javascript:Recaptcha.switch_type(\'image\')"><img src="/libraries/blankcanvas/captcha//text.png"/></a></div>
				   <div><a href="javascript:Recaptcha.showhelp()"><img src="/libraries/blankcanvas/captcha//help.png"/></a></div>
				</td>
			</tr>
			<tr>
				<td style="padding-top:.5em;">
					'.((isset( $_POST["recaptcha_challenge_field"]) && !self::passed()) ?
						'<p class="error" style="text-align:left;">Incorrect please try again</p>'
						: ''
        			).'
					
					<span class="recaptcha_only_if_image">Enter the text above:</span>
					<span class="recaptcha_only_if_audio">Enter the numbers you hear:</span>
					<input type="text" id="recaptcha_response_field" name="recaptcha_response_field" />
				</td>
				<td>
				</td>
			</tr>
		 </table>

		<script type="text/javascript" src="'. $server . '/challenge?k=' . $pubkey . '"></script>
		 <noscript>
		  <iframe src="'. $server . '/noscript?k=' . $pubkey . '" height="300" width="500" frameborder="0"></iframe><br/>
		   <textarea name="recaptcha_challenge_field" rows="3" cols="40">
		   </textarea>
		   <input type="hidden" name="recaptcha_response_field"
		        value="manual_challenge">
		 </noscript>
		';
	}
	public static function passed() {
		if(isset($_POST["recaptcha_challenge_field"])) {
			$resp = recaptcha_check_answer (self::$_privatekey,
			                                $_SERVER["REMOTE_ADDR"],
			                                $_POST["recaptcha_challenge_field"],
			                                $_POST["recaptcha_response_field"]);
			                                
			if (!$resp->is_valid) {
				return false;
			} else {
				return true;
			}
		} else {
			return false;
		}
	}
}
                                       
?>