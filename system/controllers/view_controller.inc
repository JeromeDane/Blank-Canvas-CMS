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
class ViewController extends Controller {

	public function index($params = null) {
		require_once 'system/libraries/blankcanvas/smarty_plugins/function.view.php';
		return smarty_function_view($params, null);
	}
	
	public function phpinfo($params = null) {
		
		ob_start();
		phpinfo();
		$phpinfo = ob_get_contents();
		ob_end_clean();
		
		$phpinfo = preg_replace('/<title.+?<\/title>/i', '', preg_replace('/<style/', '<div style="display:none;"', preg_replace('/<\/style>/', '</div>', $phpinfo)));
		
		$this->_view->assign('output', $phpinfo);
		
		return $this->view();
	}

}

?>