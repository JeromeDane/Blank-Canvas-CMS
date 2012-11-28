<?php
/** {@link https://github.com/JeromeDane/Blank-Canvas-CMS Blank Canvas CMS}
 *
 * This file is part of the {@link https://github.com/JeromeDane/Blank-Canvas-CMS  Blank Canvas CMS}.
 *
 * {@link https://github.com/JeromeDane/Blank-Canvas-CMS Blank Canvas CMS} is free software:
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

// fix lame $_GET glitches
if(preg_match('/\?(.*)$/', $_SERVER['REQUEST_URI'], $matches) && count($_GET) == 0) {
	$pairs = explode('&', $matches[1]);
	foreach($pairs as $pair) {
		preg_match('/^([^=]+)=(.*)/',$pair, $matches);
		$_GET[preg_replace('/\./', '_', $matches[1])] = urldecode($matches[2]);
	}
}

/**
 * Load the system
 */
require 'system/bootstrap.inc';


?>