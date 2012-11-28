<h1>user list</h1>

{foreach $users as $user}
	<p>Hello <a href="/user/{$user->id}">{$user->name}</a> - {$user->email}}</p>
{/foreach}

<p><a href="/user/create/">new user</a></p>