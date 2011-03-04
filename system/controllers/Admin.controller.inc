<?php

class AdminController extends BCController {
	
	public function index($params = null) {
		
		BC::setPageTitle("Administration");
		
		
		// connect to the database
		require 'config/db.config.inc';
		require_once 'system/utilities/phpdatamapper/Base.php';
		
		$adapter = new phpDataMapper_Adapter_Mysql($db['server'], $db['database'], $db['user'], $db['password']);
		
		
		require_once 'system/models/User.model.inc';
		
		$userModel = new SystemUserModel($adapter);
		//$userModel->migrate();
		
		$user = $userModel->get(1);
		
		
		return BC::view(array('user' => $user));
	}
	
}

?>