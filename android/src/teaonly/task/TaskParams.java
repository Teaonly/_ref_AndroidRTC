package teaonly.task;

import java.util.HashMap;

public class TaskParams {

	private HashMap<String, Object> params = null;

	public TaskParams() {
		params = new HashMap<String, Object>();
	}

	public TaskParams(String key, Object value) {
		this();
		put(key, value);
	}

	public void put(String key, Object value) {
		params.put(key, value);
	}

	public Object get(String key) {
		return params.get(key);
	}

	/**
	 * Get the boolean value associated with a key.
	 * 
	 * @param key
	 *            A key string.
	 * @return The truth. if the value is not a Boolean or the String "true" or
	 *         "false". defalut value is false.
	 */
	public boolean getBoolean(String key) {
		Object object = get(key);
		if (object.equals(Boolean.FALSE)
				|| (object instanceof String && ((String) object)
						.equalsIgnoreCase("false"))) {
			return false;
		} else if (object.equals(Boolean.TRUE)
				|| (object instanceof String && ((String) object)
						.equalsIgnoreCase("true"))) {
			return true;
		}
		return false;
	}

	/**
	 * Get the double value associated with a key.
	 * 
	 * @param key
	 *            A key string.
	 * @return The numeric value. if the value is not a Number object and cannot
	 *         be converted to a number. default value is 0.0.
	 */
	public double getDouble(String key) {
		Object object = get(key);
		try {
			return object instanceof Number ? ((Number) object).doubleValue()
					: Double.parseDouble((String) object);
		} catch (Exception e) {
			return 0.0;
		}
	}

	/**
	 * Get the int value associated with a key.
	 * 
	 * @param key
	 *            A key string.
	 * @return The integer value. be converted to an integer.
	 */
	public int getInt(String key) {
		Object object = get(key);
		try {
			return object instanceof Number ? ((Number) object).intValue()
					: Integer.parseInt((String) object);
		} catch (Exception e) {
			return 0;
		}
	}

	/**
	 * Get the string associated with a key.
	 * 
	 * @param key
	 *            A key string.
	 * @return A string which is the value.
	 */
	public String getString(String key) {
		Object object = get(key);
		return object == null ? null : object.toString();
	}

	/**
	 * Determine if the JSONObject contains a specific key.
	 * 
	 * @param key
	 *            A key string.
	 * @return true if the key exists in the JSONObject.
	 */
	public boolean has(String key) {
		return this.params.containsKey(key);
	}

}
