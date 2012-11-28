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
 */
define('BC_ROOT', getcwd());

/**
 *  auto load blank canvas classes
 */
spl_autoload_register(function($fullClassName) {
	
	
	if(preg_match('/^BlankCanvas/', $fullClassName) || __NAMESPACE__ == 'BlankCanvas') {
		
		
		// auto load base Blank Canvas classes from system library
		$className = preg_replace('/^BlankCanvas\\\\/', '', $fullClassName);
		$filePath = BC_ROOT . '/system/libraries/blankcanvas/' . $className . '_class.inc';

		if(is_file($filePath)) {
			
			require_once $filePath;
			return true;
		} else {
			
			if(preg_match('/^\\\\?BlankCanvas\\\\([a-z]+)$/i', $fullClassName, $matches)) {
				// system models
				$filePath = BC_ROOT . '/system/models/' . strtolower($className) . '_model.inc';
				if(is_file($filePath)) {
					require_once $filePath;
					return true;
				} else {
					return false;
				}
			} else
			
			// plugin models
			if(preg_match('/([a-z]+)\\\\([a-z]+)/i', $className, $matches)) {
			
				$filePath = 'plugins/'. strtolower($matches[1]) .'/models/' . strtolower($matches[2]) . '_model.inc';
				if(is_file($filePath)) {
					require_once $filePath;
				} else {
					$filePath = 'system/models/' . strtolower($matches[2]) . '_model.inc';
					if(is_file($filePath)) {
						require_once $filePath;
						
					}
				}
			}
		}
	}
});

App::init();


?>