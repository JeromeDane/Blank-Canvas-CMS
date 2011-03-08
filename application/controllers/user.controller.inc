<?php
/** {@link http://blankcanvas.me/cms/ Blank Canvas CMS}
 *
 * This file is part of the {@link http://blankcanvas.me/cms/ Blank Canvas CMS}.
 *
 * {@link http://blankcanvas.me/cms/ Blank Canvas CMS} is free software:
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
 * @author Jerome Dane <http://blankcanvas.me/contact/>
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
class UserController extends Controller {
	
	public function index($params = null) {
		
		$users = User::find();
		
		User::updateDatabase();
		
		$this->view->assign('users', $users);
		
		return $this->view->render();
		
	}
	
	public function create($params = null) {
		if(isset($_POST['name'])) {
			$user = new User();
			$user->import($_POST, "name,email,password");
			$user->save();
			App::redirect('/user/'.$user->id);
		}
		
		return $this->view->render();
	}
	
	public function details($params = null) {
		
		if($user = User::find($params['id'])) {
			
			$this->view->assign('user', $user);
			return $this->view->render();
		} else {
			return View::get404()->render();
		}
		
		
	}
	
}

?>