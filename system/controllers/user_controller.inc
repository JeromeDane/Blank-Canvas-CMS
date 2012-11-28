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

//require_once 'system/models/user_model.inc';

/**
 *
 * @author Jerome Dane
 *
 */
class UserController extends Controller {
	
	public function index($params = null) {
		
		$users =  User::find("id = 0");
		return $this->view(array(
			'users' => $users,
		));
		
	}
	
	public function logout($params = null) {
		AppUser::unload();
		App::redirect(App::getRootUri());
	}
	public function register($params = null) {
		// set continue link
		if(isset($_SERVER['HTTP_REFERER']) && !isset($_SESSION['system_user_registration_continue_link']) || !preg_match('/\/user\/register/', $_SERVER['HTTP_REFERER'])) {
			$_SESSION['system_user_registration_continue_link'] = $_SERVER['HTTP_REFERER'];
		};
		
		$errors = array();
		if(isset($_POST['register'])) {
			$email = trim($_POST['registration_email']);
			$name = trim($_POST['registration_name']);
			$password = trim($_POST['registration_password']);
			// check for unused email and username
			if(FormValidator::check('registration_email')) {
				if(User::find("email = :email", array("email" => $email))->count() > 0) {
					FormValidator::addError('registration_email', 'The email address '.$email.' is already in use.');
				}
			}
			// check for unused email and username
			if(FormValidator::check('registration_name')) {
				if(User::find("name = :name", array("name" => $name))->count() > 0) {
					FormValidator::addError('registration_name', 'The name "'.$name.'" is already in use.');
				}
			}
			if(Captcha::passed() && FormValidator::passed() && $email && $name && $password) {
				$user = new User();
				$user->import(array(
					"name" => (string) $name,
					"email" => (string) $email,
					"password" => (string) md5($password),
				));
				$user->save();
				if(AppUser::login($email, $password)) {
					App::redirect(App::getRootUri().'user/welcome');
				}
			}
		}
		return $this->view();
	}
	public function login($params = null) {
		
		$loginError = false;
		if(isset($_POST['login'])) {
			if(AppUser::getNumFailedLoginAttempts() < 3 || Captcha::passed()) {
				$success = AppUser::login($_POST['email'], $_POST['password']);
				if($success) {
					App::redirect(App::getRootUri());
				}
				$loginError = !$success;
			}
		}
		
		return $this->view(array(
			'loginError' => $loginError,
			'numLoginAttempts' => AppUser::getNumFailedLoginAttempts()
		));
	}
	
	public function welcome($params = null) {
		return $this->view(array(
			'url' => $_SESSION['system_user_registration_continue_link']
		));
	}
	public function create($params = null) {
		if (isset($_POST['name'])) {
			$user = new User();
			$user->import($_POST, "name,email,password");
			$user->save();
			App::redirect('/user/' . $user->id);
		}
		
		return $this->view();
	}
	
	public function details($params = null) {
		
		if ($user = User::findSingle("id = ".(int) $params['id'])) {
			
			$this->_view->assign('user', $user);
			return $this->view();
		} else {
			return $this->_404();;
		}
		
	}
	
}

?>