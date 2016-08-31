/*
 * Copyright 2011-2016 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

CCL_NAMESPACE_BEGIN

/* Bump Eval Nodes */

ccl_device void svm_node_enter_bump_eval(KernelGlobals *kg, ShaderData *sd, float *stack, uint offset)
{
	/* save state */
	stack_store_float3(stack, offset+0, ccl_fetch(sd, P));
	stack_store_float3(stack, offset+3, ccl_fetch(sd, dP).dx);
	stack_store_float3(stack, offset+6, ccl_fetch(sd, dP).dy);

	/* set state as if undisplaced */
	const AttributeDescriptor desc = find_attribute(kg, sd, ATTR_STD_POSITION_UNDISPLACED);

	if(desc.offset != ATTR_STD_NOT_FOUND) {
		float3 dPdx, dPdy;
		ccl_fetch(sd, P) = primitive_attribute_float3(kg, sd, desc, &dPdx, &dPdy);

		ccl_fetch(sd, dP).dx = dPdx;
		ccl_fetch(sd, dP).dy = dPdy;

		object_position_transform(kg, sd, &ccl_fetch(sd, P));
		object_position_transform(kg, sd, &ccl_fetch(sd, dP).dx);
		object_position_transform(kg, sd, &ccl_fetch(sd, dP).dy);
	}
}

ccl_device void svm_node_leave_bump_eval(KernelGlobals *kg, ShaderData *sd, float *stack, uint offset)
{
	/* restore state */
	ccl_fetch(sd, P) = stack_load_float3(stack, offset+0);
	ccl_fetch(sd, dP).dx = stack_load_float3(stack, offset+3);
	ccl_fetch(sd, dP).dy = stack_load_float3(stack, offset+6);
}

CCL_NAMESPACE_END

