<h1>New User</h1>

<form method="post">
	<p>Name <input name="name" value="{$user->name|default:""}"/></p>
	<p>Email <input name="email"/></p>
	<p>password <input name="password"/></p>
	<p><input type="submit" value="submit"/>
</form>