//
// Copyright (c) 2009, Markus Rickert
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef RL_MATH_PLUECKERTRANSFORM_HXX
#define RL_MATH_PLUECKERTRANSFORM_HXX

namespace rl
{
	namespace math
	{
		namespace spatial
		{
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			ForceVector<OtherScalar>
			PlueckerTransform<Scalar>::operator*(const ForceVector<OtherScalar>& other) const
			{
				ForceVector<OtherScalar> res;
				res.moment() = rotation().transpose() * (other.moment() - translation().cross(other.force()));
				res.force() = rotation().transpose() * other.force();
				return res;
			}
			
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			MotionVector<OtherScalar>
			PlueckerTransform<Scalar>::operator*(const MotionVector<OtherScalar>& other) const
			{
				MotionVector<OtherScalar> res;
				res.angular() = rotation().transpose() * other.angular();
				res.linear() = rotation().transpose() * (other.linear() - translation().cross(other.angular()));
				return res;
			}
			
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			RigidBodyInertia<OtherScalar>
			PlueckerTransform<Scalar>::operator*(const RigidBodyInertia<OtherScalar>& other) const
			{
				RigidBodyInertia<OtherScalar> res;
				::Eigen::Matrix<Scalar, 3, 3> px = translation().cross33();
				::Eigen::Matrix<Scalar, 3, 1> c_m_p = other.cog() - other.mass() * translation();
				res.cog() = rotation().transpose() * c_m_p;
				res.inertia() = rotation().transpose() * (other.inertia() + px * other.cog().cross33() + c_m_p.cross33() * px) * rotation();
				res.mass() = other.mass();
				return res;
			}
			
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			ArticulatedBodyInertia<OtherScalar>
			PlueckerTransform<Scalar>::operator*(const ArticulatedBodyInertia<OtherScalar>& other) const
			{
				ArticulatedBodyInertia<OtherScalar> res;
				::Eigen::Matrix<Scalar, 3, 3> px = translation().cross33();
				::Eigen::Matrix<Scalar, 3, 3> c_px_m = other.cog() - px * other.mass();
				res.cog() = rotation().transpose() * c_px_m * rotation();
				res.inertia() = rotation().transpose() * (other.inertia() - px * other.cog().transpose() + c_px_m * px) * rotation();
				res.mass() = rotation().transpose() * other.mass() * rotation();
				return res;
			}
			
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			ForceVector<OtherScalar>
			PlueckerTransform<Scalar>::operator/(const ForceVector<OtherScalar>& other) const
			{
				ForceVector<OtherScalar> res;
				res.moment() = rotation() * other.moment() + translation().cross(rotation() * other.force());
				res.force() = rotation() * other.force();
				return res;
			}
			
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			MotionVector<OtherScalar>
			PlueckerTransform<Scalar>::operator/(const MotionVector<OtherScalar>& other) const
			{
				MotionVector<OtherScalar> res;
				res.angular() = rotation() * other.angular();
				res.linear() = rotation() * other.linear() + translation().cross(rotation() * other.angular());
				return res;
			}
			
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			RigidBodyInertia<OtherScalar>
			PlueckerTransform<Scalar>::operator/(const RigidBodyInertia<OtherScalar>& other) const
			{
				RigidBodyInertia<OtherScalar> res;
				::Eigen::Matrix<Scalar, 3, 3> px = translation().cross33();
				::Eigen::Matrix<Scalar, 3, 1> R_c = rotation() * other.cog();
				::Eigen::Matrix<Scalar, 3, 1> R_c_m_p = R_c + other.mass() * translation();
				res.cog() = R_c_m_p;
				res.inertia() = rotation() * other.inertia() * rotation().transpose() - px * R_c.cross33() - R_c_m_p.cross33() * px;
				res.mass() = other.mass();
				return res;
			}
			
			template<typename Scalar>
			template<typename OtherScalar>
			inline
			ArticulatedBodyInertia<OtherScalar>
			PlueckerTransform<Scalar>::operator/(const ArticulatedBodyInertia<OtherScalar>& other) const
			{
				ArticulatedBodyInertia<OtherScalar> res;
				typename ArticulatedBodyInertia<OtherScalar>::CenterOfGravityType c = rotation() * other.cog() * rotation().transpose();
				typename ArticulatedBodyInertia<OtherScalar>::MassType m = rotation() * other.mass() * rotation().transpose();
				::Eigen::Matrix<Scalar, 3, 3> px = translation().cross33();
				::Eigen::Matrix<Scalar, 3, 3> c_px_m = c + px * m;
				res.cog() = c_px_m;
				res.inertia() = rotation() * other.inertia() * rotation().transpose() + px * c.transpose() - c_px_m * px;
				res.mass() = m;
				return res;
			}
		}
	}
}

#endif // RL_MATH_PLUECKERTRANSFORM_HXX
