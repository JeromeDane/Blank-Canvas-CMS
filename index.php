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

require 'library/smarty/libs/Smarty.class.php';
require 'library/php-activerecord/ActiveRecord.php';
require 'library/blankcanvas/App.class.inc';
require 'library/blankcanvas/Model.class.inc';
require 'library/blankcanvas/View.class.inc';
require 'library/blankcanvas/Controller.class.inc';
require 'library/blankcanvas/Exception.class.inc';



function myErrorHandler($errno, $errstr, $errfile, $errline)
{
    if (!(error_reporting() & $errno)) {
        // This error code is not included in error_reporting
        return;
    }

    switch ($errno) {
    case E_USER_ERROR:
        echo "<b>My ERROR</b> [$errno] $errstr<br />\n";
        echo "  Fatal error on line $errline in file $errfile";
        echo ", PHP " . PHP_VERSION . " (" . PHP_OS . ")<br />\n";
        echo "Aborting...<br />\n";
        exit(1);
        break;

    case E_USER_WARNING:
        echo "<b>My WARNING</b> [$errno] $errstr<br />\n";
        break;

    case E_USER_NOTICE:
        echo "<b>My NOTICE</b> [$errno] $errstr<br />\n";
        break;

    default:
        echo "Unknown error type: [$errno] $errstr<br />\n";
        break;
    }

    /* Don't execute PHP internal error handler */
    return true;
}

BlankCanvas\App::init();

?>