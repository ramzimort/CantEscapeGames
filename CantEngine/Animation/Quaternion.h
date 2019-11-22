/**
  * @file Quaternion.h
  * @author Jose Rosenbluth
  * @brief Quaternion math class (for dealing with animation and orientation)
  * @date 2019-11-21
  */
#pragma once

#define SQRT_EPSILON 0.001f


namespace MathUtil
{
	/**
	 * @brief Quaternion math class (for dealing with animation and orientation)
	 * 
	 */
	struct Quaternion 
	{
		/**
		 * @brief Override operation * for scalar multiplication
		 * 
		 * @param f 
		 * @param rhs 
		 * @return Quaternion 
		 */
		friend Quaternion operator*(float f, Quaternion const& rhs);
		/**
		 * @brief Override operator * for scalar multiplication
		 * 
		 * @param rhs 
		 * @param f 
		 * @return Quaternion 
		 */
		friend Quaternion operator*(Quaternion const& rhs, float f);
		/**
		 * @brief Override operator / for scalar division
		 * 
		 * @param rhs 
		 * @param f 
		 * @return Quaternion 
		 */
		friend Quaternion operator/(Quaternion const& rhs, float f);
		/**
		 * @brief Override operator / for scalar division
		 * 
		 * @param rhs 
		 * @param f 
		 * @return Quaternion 
		 */
		friend Quaternion operator/(float f, Quaternion const& rhs);

		//Data
		/**
		 * @brief Real part of the quaternion
		 * 
		 */
		float s;
		/**
		 * @brief Complex part of the quaternion
		 * 
		 */
		float x, y, z;

		/**
		 * @brief Construct a new Quaternion object
		 * 
		 */
		Quaternion() : s(0.0f), 
			x(0.0f), y(0.0f), z(0.0f) 
		{}

		/**
		 * @brief Construct a new Quaternion object
		 * 
		 * @param s 
		 * @param x 
		 * @param y 
		 * @param z 
		 */
		Quaternion(float s, float x, 
			float y, float z) : s(s),
			x(x), y(y), z(z)
		{}

		/**
		 * @brief Construct a new Quaternion object
		 * 
		 * @param s 
		 * @param v 
		 */
		Quaternion(float s, Vector3 const& v) : s(s),
			x(v.x), y(v.y), z(v.z)
		{}

		/**
		 * @brief Construct a new Quaternion object
		 * 
		 * @param q 
		 */
		Quaternion(Vector4 const& q) : s(q.w),
			x(q.x), y(q.y), z(q.z)
		{}

		/**
		 * @brief Construct a new Quaternion object
		 * 
		 * @param rhs 
		 */
		Quaternion(Quaternion const& rhs) : s(rhs.s),
			x(rhs.x), y(rhs.y), z(rhs.z)
		{}

		/**
		 * @brief Assignment operator override
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion& operator=(Quaternion const& rhs)
		{
			if (this != &rhs) 
			{
				this->s = rhs.s;
				this->x = rhs.x;
				this->y = rhs.y;
				this->z = rhs.z;
			}
			return *this;
		}

		/**
		 * @brief Assignment operator override
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion& operator=(Vector4 const& rhs)
		{
			this->s = rhs.w;
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			return *this;
		}

		/**
		 * @brief Gets a quaternion equivalent to the axis angle rotation described by angle in degrees and a 3 dimensional axis
		 * 
		 * @param degree 
		 * @param axis 
		 * @return Quaternion 
		 */
		static Quaternion QuaternionFromAngleAxisA(float degree, Vector3 const& axis) 
		{
			float radians = DirectX::XMConvertToRadians(degree);
			float temp = radians / 2;
			Vector3 A = axis;
			A.Normalize();
			return Quaternion(cosf(temp), sinf(temp) * A);
		}

		/**
		 * @brief Override bracket operator for quaternion
		 * 
		 * @param index 
		 * @return float 
		 */
		float operator[](unsigned int index) 
		{
			assert(index >= 0 && index < 4);

			switch (index) 
			{
			case 0:
				return this->x;
			case 1:
				return this->y;
			case 2:
				return this->z;
			case 3:
				return this->s;
			};
		}
		//Second case (not yet)
		
		//////////////////////////
		////    Operations    ////
		//////////////////////////

		/**
		 * @brief Override plus equal operator
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion& operator+=(Quaternion const& rhs)
		{
			this->s += rhs.s;
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}

		/**
		 * @brief Override plus equal operator
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion const operator+(Quaternion const& rhs) const
		{
			Quaternion copy = Quaternion(*this);
			copy += rhs;
			return copy;
		}
		
		/**
		 * @brief Override times equal operator
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion& operator*=(Quaternion const& rhs)
		{
			Vector3 v1(x, y, z);
			Vector3 v2(rhs.x, rhs.y, rhs.z);
			Vector3 v(s * v2 + rhs.s * v1 + v1.Cross(v2));
			this->s = (s * rhs.s) - (x * rhs.x + y * rhs.y + z * rhs.z);
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;

			return *this;
		}
		/**
		 * @brief Override times equal operator
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion& operator*=(float scalar)
		{
			this->s *= scalar;
			this->x *= scalar;
			this->y *= scalar;
			this->z *= scalar;
			return *this;
		}
		/**
		 * @brief Override division equal operator
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion& operator/=(float scalar)
		{
			this->s /= scalar;
			this->x /= scalar;
			this->y /= scalar;
			this->z /= scalar;
			return *this;
		}
		
		/**
		 * @brief Override unary operator - (negation of the quaternion)
		 * 
		 * @param rhs 
		 * @return Quaternion& 
		 */
		Quaternion operator-() const
		{
			Quaternion q;
			q.s = -this->s;
			q.x = -this->x;
			q.y = -this->y;
			q.z = -this->z;
			return q;
		}

		/**
		 * @brief Overrides operator * for quaternion on quaternion multiplication
		 * 
		 * @param rhs 
		 * @return Quaternion 
		 */
		Quaternion operator*(Quaternion const& rhs) const
		{
			Quaternion copy = Quaternion(*this);
			copy *= rhs;
			return copy;
		}

		/**
		 * @brief Overrides operator * for scalar multiplication
		 * 
		 * @param rhs 
		 * @return Quaternion 
		 */
		Quaternion operator*(float scalar)
		{
			Quaternion copy = Quaternion(*this);
			copy *= scalar;
			return copy;
		}


		/**
		 * @brief Overrides operator / for scalar division
		 * 
		 * @param rhs 
		 * @return Quaternion 
		 */
		Quaternion operator/(float scalar)
		{
			Quaternion copy = Quaternion(*this);
			copy /= scalar;
			return copy;
		}
		
		void print(std::string name)
		{
			///std::cout << name << ": [" << this->s << ", " << this->x << ", " << this->y << ", " << this->z << "]" << std::endl;
		}


		/**
		 * @brief Returns the conjugate of the quaternion
		 * 
		 * @return Quaternion 
		 */
		Quaternion Conjugate() const 
		{
			return Quaternion(s, -x, -y, -z);
		}
		
		/**
		 * @brief Returns the inverse of the quaternion
		 * 
		 * @return Quaternion 
		 */
		Quaternion Inverse() const
		{
			if (IsUnitQuaternion())
				return Conjugate();

			return Conjugate() / SqrLen();
		}

		/**
		 * @brief Returns the quaternion normalized
		 * 
		 * @return Quaternion 
		 */
		Quaternion Normalize() const
		{
			if (IsUnitQuaternion())
				return Quaternion(s, x, y, z);

			return Quaternion(s, x, y, z) / Len();
		}

		/**
		 * @brief Square length of the quaternion
		 * 
		 * @return float 
		 */
		float SqrLen() const
		{
			return s * s + x * x + y * y + z * z;
		}
		
		/**
		 * @brief Length of the quaternion
		 * 
		 * @return float 
		 */
		float Len() const
		{
			return sqrtf( this->SqrLen() );
		}

		/**
		 * @brief Returns true if the quaternions length is one
		 * 
		 * @return true 
		 * @return false 
		 */
		bool IsUnitQuaternion() const
		{
			return fabs(SqrLen() - 1.0f) <= SQRT_EPSILON;
		}

		/**
		 * @brief defined dot product for the quaternion
		 * 
		 * @param rhs 
		 * @return float 
		 */
		float Dot(Quaternion const& rhs) const
		{
			return (s * rhs.s) + (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
		}

		//Matrix
		/// _11, _12, _13, _14 : First row of matrix
		/// _21, _22, _23, _24 : Second row of matrix
		/// _31, _32, _33, _34 : Third row of matrix
		/// _41, _42, _43, _44 : Fourth row of matrix
		
		/**
		 * @brief Get the Rotation Matrix based on this quaternion object
		 * 
		 * @return Matrix 
		 */
		Matrix GetRotationMatrix() const 
		{
			float s2 = s * s;
			float x2 = x * x;
			float y2 = y * y;
			float z2 = z * z;

			float _2sx = 2 * s * x;
			float _2sy = 2 * s * y;
			float _2sz = 2 * s * z;

			float _2xy = 2 * x * y;
			float _2xz = 2 * x * z;
			float _2yz = 2 * y * z;

			Matrix matrix = Matrix::Identity;

			matrix._11 = 1 - 2 * (y2 + z2);
			matrix._22 = 1 - 2 * (x2 + z2);
			matrix._33 = 1 - 2 * (x2 + y2);

			//column major
			/*matrix._12 = _2xy - _2sz;
			matrix._13 = _2xz + _2sy;

			matrix._21 = _2xy + _2sz;
			matrix._23 = _2yz - _2sx;

			matrix._31 = _2xz - _2sy;
			matrix._32 = _2yz + _2sx;*/

			matrix._21 = _2xy - _2sz;
			matrix._31 = _2xz + _2sy;

			matrix._12 = _2xy + _2sz;
			matrix._32 = _2yz - _2sx;

			matrix._13 = _2xz - _2sy;
			matrix._23 = _2yz + _2sx;

			return matrix;
		}
		
		/**
		 * @brief Static method. Returns a the identity quaternion
		 * 
		 * @return Quaternion 
		 */
		static Quaternion Unit()
		{
			return Quaternion (1, 0, 0, 0);
		}

		/**
		 * @brief Rotates vector r, in degrees, around axis and returns it
		 * 
		 * @param degrees 
		 * @param axis 
		 * @param r 
		 * @return Vector3 
		 */
		static Vector3 Rotate(float degrees, Vector3 const& axis, Vector3 const& r)
		{
			float radians = DirectX::XMConvertToRadians(degrees);
			float temp = radians / 2;
			Vector3 A = axis;
			A.Normalize();
			
			Quaternion q = Quaternion(cosf(temp), sinf(temp) * A);
			Quaternion result = q * Quaternion(0, r) * q.Conjugate();

			Vector3 new_r = Vector3(result.x, result.y, result.z);
			new_r.Normalize();
			return new_r;
		}

		/**
		 * @brief Applies quaternion rotation to vector r and returns it
		 * 
		 * @param q 
		 * @param r 
		 * @return Vector3 
		 */
		static Vector3 Rotate(Quaternion const& q, Vector3 const& r)
		{
			Quaternion result = q * Quaternion(0, r) * q.Conjugate();
			
			Vector3 new_r = Vector3(result.x, result.y, result.z);
			new_r.Normalize();
			return new_r;
		}
	};
}
