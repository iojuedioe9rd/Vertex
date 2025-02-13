timer = 0

function update(ts)
	timer = timer + ts
	vec = get_pos()
	if is_key_pressed(KeyCode.W) then
		vec.y = vec.y + 5 * ts
		end
	if is_key_pressed(KeyCode.S) then
		vec.y = vec.y - 5 * ts
	end
	
	if is_key_pressed(KeyCode.A) then
		vec.x = vec.x - 5 * ts
	end
	
	if is_key_pressed(KeyCode.D) then
		vec.x = vec.x + 5 * ts
	end
	set_pos(vec)
end