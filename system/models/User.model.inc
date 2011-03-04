<?php


/**
 *
 * Enter description here ...
 * @author Jerome
 *
 * @method SystemUserEntity get()
 */
class SystemUserModel extends phpDataMapper_Base {
	
	protected $_datasource = "system_users";
	protected $_entityClass = 'SystemUserEntity';
	
	
	public $id = array(
		'type'    => 'int',
		'primary' => true,
		'serial'  => true
	);
	
	public $email = array(
		"type"     => "string",
		"required" => true
	);
	
	public $name = array(
		"type"     => "string",
		"required" => true
	);
	
	
}

/**
 *
 * Enter description here ...
 * @author Jerome
 *
 * @property string $id The user's primary key
 * @property string $email The user's email address
 * @property string $name The user's [screen]name
 */
class SystemUserEntity extends phpDataMapper_Entity
{
    public function getGreeting()
    {
        return "hello ".$this->name." from ".$this->email;
    }
}


?>