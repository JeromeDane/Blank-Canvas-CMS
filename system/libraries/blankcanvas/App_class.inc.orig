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
 * Main application controller and system interface
 * @author Jerome Dane
 *
 */
class App {

	/**
	 *
	 */
	protected static $_version = '3.0.0a1pre';

	/**
	 * Initialize
	 *
	 * Initializes the system. Only called once by /index.php
	 *
	 */
	public static function init() {

		AppUser::startSession();
		
		require 'data/system/config.inc';
		DataAdapter::connect('mysql:host='.$db['server'].';dbname='.$db['database'], $db['user'], $db['password']);

		App::processRequest();

	}

	/**
	 * Get the request URI without the domain
	 *
	 * @return string The base path of the request without the domain, but including the leading '/'
	 */
	public static function getRequestUri() {
		$location = preg_replace('/\/404.php\?404;https?:\/\/' . $_SERVER['HTTP_HOST'] . ':[0-9]*/i', '', $_SERVER['REQUEST_URI']);
		// strip root path
		$escapedRootUri = str_replace('/', '\/', preg_quote(App::getRootUri()));
		$location = preg_replace('/^' . $escapedRootUri . '/', '', $location);
		return '/' . $location;
	}

	/**
	 * Returns the site's root URL path not including the domain
	 *
	 * @return string site URL root without the leading http://example.com
	 */
	public static function getRootUri() {
		static $root;
		if (is_null($root)) {
			preg_match('/^(.*?\/)index.php/i', $_SERVER['PHP_SELF'], $matches);
			$root = $matches ? $matches[1] : '/';
		}
		return $root;
	}

	public static function getVersion() {
		return self::$_version;
	}

	/**
	 * Get Request Token
	 *
	 * @param int $index
	 */
	public static function getRequestToken($index) {
		$tokens = self::$_requestTokens;
		return isset($tokens[$index]) ? $tokens[$index] : null;
	}
	/**
	 * Get Request Var
	 *
	 * @param int $index
	 */
	public static function getRequestVar($index) {
		$vars = self::$_requestVars;
		return isset($vars[$index]) ? $vars[$index] : null;
	}

	public static function getRequestTokents() {
		return self::$_requestTokens;
	}
	public static function getRequestVars() {
		return self::$_requestVars;
	}

	/**
	 * Redirect the user to a new URI
	 *
	 */
	public static function redirect($uri) {
		//header("Status: 301 Moved Permanently");
		//header('HTTP/1.0 301 Moved Permanently');
		$uri = preg_replace('/^\//', '/', $uri);
		header("Location: " . $uri);
		App::close();
		die();
	}

	/**
	 * Set Controller
	 * Sets the current controller
	 * @param string $controllerName the name of the controller to use
	 * @return void
	 */
	public static function setController($controllerName) {
		$controllerName = strtolower($controllerName);
		$systemControllerFile = 'system/controllers/' . $controllerName . '.controller.inc';
		if (is_file($systemControllerFile)) {
			require_once $systemControllerFile;
			// make sure controller class exists in file
			$className = '\\BlankCanvas\\' . ucfirst($controllerName) . 'Controller';
			if (!class_exists($className) && $controllerName != '404') {
				throw new Exception('Controller file found at /' . $systemControllerFile . ' but it
									does not contain a valid controller class named "' . $className . '"');
			}
			$controller = $controllerName == '404' ? new FileNotFoundController() : new $className();
				
			if (!is_subclass_of($controller, '\\BlankCanvas\\Controller')) {
				throw new Exception($className . '" class found in /' . $systemControllerFile . ' but it does not extend from BCController');
			}
		} else {
				
		}
		if (!isset($controller)) {
			App::setController('404');
		} else {
			self::$_controller = $controller;
			self::$_controllerName = $controllerName;
				
			// define default routes and call action accordingly
			$token = App::getRequestToken(1);
			if (is_null($token)) {
				App::callAction('index');
			} else if (preg_match('/^\d+$/', $token, $matches)) {
				App::callAction('details', array('id' => (int) $matches[0]));
			} else {
				App::callAction($token);
			}
		}
	}

	/**
	 * Process Request
	 *
	 * Process a request and output the result if generated
	 *
	 * @param string $requestUri
	 * @return void
	 * @todo consider removing controller inherits from \BlankCanvas\Controller check to improve performance
	 * @todo divide this up into smaller parts
	 */
	public static function processRequest($requestUri = null) {

		self::$_requestUri = is_null($requestUri) ? App::getRequestUri() : $requestUri;
		self::_generateRequestTokens();

		$tokens = App::getRequestTokents();
		$params = array();
		
		$firstToken = preg_match('/^\d+$/', App::getRequestToken(0)) ? 'content' : App::getRequestToken(0);

		if ($controller = self::getController('system', $firstToken)) {
				
			$actionTokenIndex = 1;
				
		} else {
				
			// check for plugin controller
			$secondToken = App::getRequestToken(1) ? App::getRequestToken(1) : 'index';
			if(preg_match('/^\d+$/', $secondToken)) {
				$controller = self::getController($firstToken, 'index');
				$actionTokenIndex = 1;
			} else if($controller = self::getController($firstToken, $secondToken)) {
				$actionTokenIndex = 2;
			} else {
				$controller = self::getController($firstToken, 'index');
				$actionTokenIndex = 1;
			}
		}
		
		// determine action name
		if($controller) {
			// determine request tokens and action name
			if (!App::getRequestToken($actionTokenIndex)) { // index
				$actionName = 'index';
			} else if (preg_match('/^\d+$/', App::getRequestToken($actionTokenIndex))) {
				$actionName = 'details';
				$params['id'] = (int) App::getRequestToken($actionTokenIndex);
			} else {
				$actionName = App::getRequestToken($actionTokenIndex);
				if(preg_match('/^\d+$/', App::getRequestToken($actionTokenIndex + 1))) {
					$params['id'] = (int) App::getRequestToken($actionTokenIndex + 1);
				}
			}
		}
		
		if($controller) {
			// make sure the controller extends from \BlankCanvas\Controller
			if (!is_subclass_of($controller, '\\BlankCanvas\\Controller')) {
				throw new Exception($className . '" class found in /' . $systemControllerFile . ' but it does not extend from BCController');
			}
			if(method_exists($controller, $actionName)) {
				echo ($controller->$actionName(array_merge($params, self::getRequestVars())));
			} else if(method_exists($controller, 'index')) {
				echo ($controller->index(array_merge($params, self::getRequestVars())));
			}
		} else {
			echo (View::get404());
		}
		
		session_write_close();

	}

	public static function getController($context, $controllerName) {
		
		$filePath = BC_ROOT . '/' . ($context == 'system' ? 'system' : 'plugins/' . $context);
		$filePath .= '/controllers/' . $controllerName . '_controller.inc';

		if(is_file($filePath)) {
			// make sure controller class exists
			require_once $filePath;
			$className = 'BlankCanvas\\' . $context . '\\' . ucfirst($controllerName) . 'Controller';
			if (!class_exists($className) && $controllerName != '404') {
				$className = 'BlankCanvas\\' . ucfirst($controllerName) . 'Controller';
			}
			if (!class_exists($className) && $controllerName != '404') {
				throw new Exception('Controller file found at /' . $className . ' but it
									does not contain a valid controller class named "' . $className . '"');
			} else {
				return new $className($context);
			}
		} else {
			return false;
		}


	}

	/**
	 * Container for the current request uri
	 * @var string
	 */
	protected static $_requestUri;

	/**
	 * Container for the current request tokens
	 * @var array
	 */
	protected static $_requestTokens;

	/**
	 * Container for the current request variables
	 * @var array $_requestVars
	 */
	protected static $_requestVars;


	private static function _getSystemController($controllerName) {

		$filePath = BC_ROOT . '/system/controllers/' . $controllerName . '_controller.inc';
		if(is_file($filePath)) {
			// make sure controller class exists
			require_once $filePath;
			$className = '\\BlankCanvas\\' . ucfirst($controllerName) . 'Controller';
			if (!class_exists($className) && $controllerName != '404') {
				throw new Exception('Controller file found at /' . $className . ' but it
									does not contain a valid controller class named "' . $className . '"');
			} else {
				return new $className();
			}
		} else {
			return false;
		}
	}

	/**
	 * Add styles to skin html
	 *
	 * Takes a string representing the HTML code for the template and inserts the
	 * contents of all CSS files into the <head> tag. This is done in order to ensure
	 * that root level urls are correctly parsed.
	 * @param string $skinHtml The HTML string for the skin template
	 * @return string
	 */
	/*
	private static function _addStylesToSkinHtml($skinHtml) {
		$style = '';
		$links = '';
		foreach (self::$_stylesheetPaths as $src => $trash) {
			if (is_file($src)) {
				$styleContents = file_get_contents($src);
				if (App::getRootUri() != '/') {
					$style .= "\n\t" . '<style type="text/css">' . preg_replace('/url\(("|\')?\//i', "url(\$1" . App::getRootUri(), $styleContents) . '</style>' . "\n";
				} else {
					$links .= "\n\t" . '<link rel="stylesheet" type="text/css" href="/' . $src . '" /> ';
				}
			}
		}
		$outputHtml = preg_replace('/<\/head>/i', $style . '</head>', $skinHtml, 1);
		$outputHtml = preg_replace('/<\/head>/i', $links . '</head>', $outputHtml, 1);
		return $outputHtml;
	}
	*/

	public static function close() {
		
	}
	
	
	protected function _generateRequestTokens() {
		self::$_requestVars = array();
		// generate rough tokens
		$roughTokens = array();
		if (preg_match('/^\/(index\.php)?$/', self::$_requestUri)) { // on the home page
			$roughTokens = array(
				'index'
				);
		} else {
			$roughTokens = preg_split('/\/|\?/', self::$_requestUri); // handle normal urls
		}
		// parse rough tokens
		$tokens = array();
		foreach ($roughTokens as $roughToken) {
			if (trim($roughToken) != '') {
				$tokens[] = trim(urldecode(preg_replace('/(%2f|\/)/i', '', $roughToken)));
			}
		}
		self::$_requestTokens = $tokens;
		foreach (self::$_requestTokens as $token) {
			if (preg_match('/^([^_]+)_([^\/]*)$/', $token, $matches)) {
				self::$_requestVars[$matches[1]] = $matches[2];
			}
		}
	}
}

?>