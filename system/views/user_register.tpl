<h1>User Registration</h1>


{form}
	{form_errors}
	<table class="user_login_layout" style="margin-top:2em;">
		<tr>
			<th>{label for="registration_email"}Email{/label}</th>
			<td>{input name="registration_email" 
					required="Please enter your email address"
					email="The email address you entered appears to be invalid"}
				- Used for login and password recovery. Never made public.
			</td>
		</tr>
		<tr>
			<th></th>
			<td>{input name="registration_email_match" 
					required="Please confirm your email address" 
					match="The email confirmation does not match"}
				{label for="registration_email_match"} - Confirm email address above{/label}
			</td>
		</tr>
		<tr>
			<th>{label for="registration_password"}Password{/label}</th>
		 	<td>{input name="registration_password" type="password"
		 			required="Please choose a password"
		 			minimum="Your password must be at least 6 characters long"}
		 		- Choose a password of at least six characters
		 	</td>
		</tr>
		<tr>
			<th></th>
		 	<td>{input name="registration_password_match" type="password"
		 			match="The passwords you entered do not match"}
		 		{label for="registration_password_match"}- Confirm your password{/label}
		 	</td>
		</tr>
		<tr>
			<th>{label for="registration_name"}Name{/label}</th>
		 	<td>{input name="registration_name"
		 			required="Please enter a name"
		 			minlength="Your name must be at least 3 characters long"
		 			maxlength="Your name may not be longer than 100 characters"}
		 		- This is how others will identify you on the site
		 	</td>
		</tr>
		<tr>
			<th></th>
		 	<td><br/>{html_captcha}</td>
		</tr>
		<tr>
			<th></th>
		 	<td>
		 		{submit value="Register" name="register"}
		 	</td>
		</tr>
	</table>
	
{/form}
