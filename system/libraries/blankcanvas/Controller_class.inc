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


class Controller {
	
	protected $_view;
	protected $_context;
	protected $_name;
	
	public function __construct($context = 'system') {
		
		$this->_context = $context;
		
		// determine simple controller name
		$traceStack = debug_backtrace();
		$className = get_class($traceStack[0]['object']);
		$this->_name = strtolower(preg_replace('/Controller$/', '', $className));
		$this->_name = preg_replace('/^.+\\\\/', '', $this->_name);
		
		// create view
		$this->_view = new View($this->_context, $this->_name);
		
	}
	
	public function view(array $params = array(), $useLayout = true) {
		if(!$useLayout) {
			$this->_view->setLayoutFile(null);
		}
		
		return $this->_view->render($params);
	}
	protected function _404($params = null) {
		return View::get404();
	}
	protected function _denied($params = null) {
		return 'access denied';
	}
}

?>