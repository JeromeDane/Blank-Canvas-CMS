<?php
/** {@link http://jeromedane.com Blank Canvas CMS}
 *
 * This file is part of the {@link http://jeromedane.com Blank Canvas CMS}.
 *
 * {@link http://jeromedane.com Blank Canvas CMS} is free software:
 * you can redistribute it and/or modify it under the terms of the
 * {@link http://www.gnu.org/licenses/gpl.html GNU General Public License}
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Jerome Dane <http://jeromedane.com>
 * @copyright Copyright (c) 2011, Jerome Dane
 * @license http://www.gnu.org/licenses/gpl.html GNU General Public License
 * @package BlankCanvas
 */

namespace BlankCanvas;


/**
 *
 * @author Jerome Dane
 *
 */
class IndexController extends Controller {
	
	public function index($params = null) {
		
		
		return $this->view();
	}
	public function execute($params = null) {
		
		require_once 'plugins/googlelogin/libraries/lightopenid/openid.inc';
		try {
		    # Change 'localhost' to your domain name.
		    $openid = new \LightOpenID($_SERVER['HTTP_HOST']);
		    
		    if(!$openid->mode) {
		    		if($_SERVER['HTTP_REFERER']) {
		    			$_SESSION['googlelogin_referer'] =  $_SERVER['HTTP_REFERER'];
		    		} else {
		    			$_SESSION['googlelogin_referer'] = isset($_SESSION['googlelogin_referer']) ? $_SESSION['googlelogin_referer'] : App::getRootUri();
		    		}
		            $openid->identity = 'https://www.google.com/accounts/o8/id';
		            $openid->required = array('namePerson/first', 'namePerson/last', 'contact/email');
		            header('Location: ' . $openid->authUrl());
		    } elseif($openid->mode == 'cancel') {
		        App::redirect(App::getRootUri().'googlelogin/canceled/');
		    } else {
		        if($openid->validate()) {
		        	$user = User::findSingle('googleIdentity = :googleIdentity', array('googleIdentity' => $openid->identity));
		        	if(!$user) {
		        		$attributes = $openid->getAttributes();
		        	    $email = $attributes['contact/email'];
		    	        $first_name = $attributes['namePerson/first'];
			            $last_name = $attributes['namePerson/last'];
			            // try to load existing user by email
			            $user = User::findSingle('email = :email', array('email' => $email));
			            if(!$user) {
			            	$_SESSION['googlelogin'] = array(
			            		"password" => md5(md5(rand(0, 9999)).$email),	// create some random unknowable password
			            		"name" => $first_name.' '.$last_name,
			            		"email" => $email,
			            		"identity" => $openid->identity
			            	);
		        			App::redirect(App::getRootUri().'googlelogin/register/');
			            }
		        		$user->googleIdentity = $openid->identity;
		        		$user->save();
		        	}
	        		AppUser::load($user);
	        		App::redirect($_SESSION['googlelogin_referer']);
		        }
		    }
		} catch(ErrorException $e) {
		    echo $e->getMessage();
		}
				
		//return $this->view();
		
	}
	
	public function register($params = null) {
		if(isset($_POST['create'])) {
			$name = trim($_POST['registration_name']);
			// check for unused username
			if(FormValidator::check('registration_name')) {
				if(User::find("name = :name", array("name" => $name))->count() > 0) {
					FormValidator::addError('registration_name', 'The name "'.$name.'" is already in use.');
				}
			}
			if(FormValidator::passed()) {
				$user = new User();
				$user->email = $_SESSION['googlelogin']['email'];
				$user->name = $name;
				$user->password = $_SESSION['googlelogin']['password'];
	        	$user->googleIdentity =  $_SESSION['googlelogin']['identity'];
				$user->save();
				AppUser::load($user);
		        App::redirect($_SESSION['googlelogin_referer']);
			}
		}
		
		if(!isset($_POST['registration_name'])) $_POST['registration_name'] = $_SESSION['googlelogin']['name']; 
		return $this->view(array(
			'email' => $_SESSION['googlelogin']['email'],
			'numLoginAttempts' => AppUser::getNumFailedLoginAttempts()
		));
	}
	
}

?>