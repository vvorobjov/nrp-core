#include "pch-cpp.hpp"

#ifndef _MSC_VER
# include <alloca.h>
#else
# include <malloc.h>
#endif


#include <limits>


template <typename T1>
struct VirtualActionInvoker1
{
	typedef void (*Action)(void*, T1, const RuntimeMethod*);

	static inline void Invoke (Il2CppMethodSlot slot, RuntimeObject* obj, T1 p1)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		((Action)invokeData.methodPtr)(obj, p1, invokeData.method);
	}
};
template <typename T1, typename T2>
struct VirtualActionInvoker2
{
	typedef void (*Action)(void*, T1, T2, const RuntimeMethod*);

	static inline void Invoke (Il2CppMethodSlot slot, RuntimeObject* obj, T1 p1, T2 p2)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		((Action)invokeData.methodPtr)(obj, p1, p2, invokeData.method);
	}
};
template <typename T1, typename T2, typename T3>
struct VirtualActionInvoker3
{
	typedef void (*Action)(void*, T1, T2, T3, const RuntimeMethod*);

	static inline void Invoke (Il2CppMethodSlot slot, RuntimeObject* obj, T1 p1, T2 p2, T3 p3)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		((Action)invokeData.methodPtr)(obj, p1, p2, p3, invokeData.method);
	}
};
template <typename T1, typename T2, typename T3, typename T4>
struct VirtualActionInvoker4
{
	typedef void (*Action)(void*, T1, T2, T3, T4, const RuntimeMethod*);

	static inline void Invoke (Il2CppMethodSlot slot, RuntimeObject* obj, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		((Action)invokeData.methodPtr)(obj, p1, p2, p3, p4, invokeData.method);
	}
};
template <typename R>
struct VirtualFuncInvoker0
{
	typedef R (*Func)(void*, const RuntimeMethod*);

	static inline R Invoke (Il2CppMethodSlot slot, RuntimeObject* obj)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		return ((Func)invokeData.methodPtr)(obj, invokeData.method);
	}
};
template <typename R, typename T1>
struct VirtualFuncInvoker1
{
	typedef R (*Func)(void*, T1, const RuntimeMethod*);

	static inline R Invoke (Il2CppMethodSlot slot, RuntimeObject* obj, T1 p1)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		return ((Func)invokeData.methodPtr)(obj, p1, invokeData.method);
	}
};
template <typename R, typename T1, typename T2>
struct VirtualFuncInvoker2
{
	typedef R (*Func)(void*, T1, T2, const RuntimeMethod*);

	static inline R Invoke (Il2CppMethodSlot slot, RuntimeObject* obj, T1 p1, T2 p2)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		return ((Func)invokeData.methodPtr)(obj, p1, p2, invokeData.method);
	}
};
template <typename R, typename T1, typename T2, typename T3, typename T4>
struct VirtualFuncInvoker4
{
	typedef R (*Func)(void*, T1, T2, T3, T4, const RuntimeMethod*);

	static inline R Invoke (Il2CppMethodSlot slot, RuntimeObject* obj, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_virtual_invoke_data(slot, obj);
		return ((Func)invokeData.methodPtr)(obj, p1, p2, p3, p4, invokeData.method);
	}
};
template <typename R>
struct GenericVirtualFuncInvoker0
{
	typedef R (*Func)(void*, const RuntimeMethod*);

	static inline R Invoke (const RuntimeMethod* method, RuntimeObject* obj)
	{
		VirtualInvokeData invokeData;
		il2cpp_codegen_get_generic_virtual_invoke_data(method, obj, &invokeData);
		return ((Func)invokeData.methodPtr)(obj, invokeData.method);
	}
};
template <typename R, typename T1>
struct GenericVirtualFuncInvoker1
{
	typedef R (*Func)(void*, T1, const RuntimeMethod*);

	static inline R Invoke (const RuntimeMethod* method, RuntimeObject* obj, T1 p1)
	{
		VirtualInvokeData invokeData;
		il2cpp_codegen_get_generic_virtual_invoke_data(method, obj, &invokeData);
		return ((Func)invokeData.methodPtr)(obj, p1, invokeData.method);
	}
};
struct InterfaceActionInvoker0
{
	typedef void (*Action)(void*, const RuntimeMethod*);

	static inline void Invoke (Il2CppMethodSlot slot, RuntimeClass* declaringInterface, RuntimeObject* obj)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_interface_invoke_data(slot, obj, declaringInterface);
		((Action)invokeData.methodPtr)(obj, invokeData.method);
	}
};
template <typename R>
struct InterfaceFuncInvoker0
{
	typedef R (*Func)(void*, const RuntimeMethod*);

	static inline R Invoke (Il2CppMethodSlot slot, RuntimeClass* declaringInterface, RuntimeObject* obj)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_interface_invoke_data(slot, obj, declaringInterface);
		return ((Func)invokeData.methodPtr)(obj, invokeData.method);
	}
};
template <typename R, typename T1>
struct InterfaceFuncInvoker1
{
	typedef R (*Func)(void*, T1, const RuntimeMethod*);

	static inline R Invoke (Il2CppMethodSlot slot, RuntimeClass* declaringInterface, RuntimeObject* obj, T1 p1)
	{
		const VirtualInvokeData& invokeData = il2cpp_codegen_get_interface_invoke_data(slot, obj, declaringInterface);
		return ((Func)invokeData.methodPtr)(obj, p1, invokeData.method);
	}
};
template <typename R>
struct GenericInterfaceFuncInvoker0
{
	typedef R (*Func)(void*, const RuntimeMethod*);

	static inline R Invoke (const RuntimeMethod* method, RuntimeObject* obj)
	{
		VirtualInvokeData invokeData;
		il2cpp_codegen_get_generic_interface_invoke_data(method, obj, &invokeData);
		return ((Func)invokeData.methodPtr)(obj, invokeData.method);
	}
};
template <typename R, typename T1>
struct GenericInterfaceFuncInvoker1
{
	typedef R (*Func)(void*, T1, const RuntimeMethod*);

	static inline R Invoke (const RuntimeMethod* method, RuntimeObject* obj, T1 p1)
	{
		VirtualInvokeData invokeData;
		il2cpp_codegen_get_generic_interface_invoke_data(method, obj, &invokeData);
		return ((Func)invokeData.methodPtr)(obj, p1, invokeData.method);
	}
};
template <typename R, typename T1>
struct InvokerFuncInvoker1;
template <typename R, typename T1>
struct InvokerFuncInvoker1<R, T1*>
{
	static inline R Invoke (Il2CppMethodPointer methodPtr, const RuntimeMethod* method, void* obj, T1* p1)
	{
		R ret;
		void* params[1] = { p1 };
		method->invoker_method(methodPtr, method, obj, params, &ret);
		return ret;
	}
};
template <typename R, typename T1, typename T2>
struct InvokerFuncInvoker2;
template <typename R, typename T1, typename T2>
struct InvokerFuncInvoker2<R, T1*, T2*>
{
	static inline R Invoke (Il2CppMethodPointer methodPtr, const RuntimeMethod* method, void* obj, T1* p1, T2* p2)
	{
		R ret;
		void* params[2] = { p1, p2 };
		method->invoker_method(methodPtr, method, obj, params, &ret);
		return ret;
	}
};
template <typename R, typename T1, typename T2, typename T3>
struct InvokerFuncInvoker3;
template <typename R, typename T1, typename T2, typename T3>
struct InvokerFuncInvoker3<R, T1*, T2*, T3*>
{
	static inline R Invoke (Il2CppMethodPointer methodPtr, const RuntimeMethod* method, void* obj, T1* p1, T2* p2, T3* p3)
	{
		R ret;
		void* params[3] = { p1, p2, p3 };
		method->invoker_method(methodPtr, method, obj, params, &ret);
		return ret;
	}
};

// System.Action`1<System.Object>
struct Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87;
// System.Action`1<Grpc.Core.ServiceBinderBase>
struct Action_1_t805B4C224C6B55AF87988396EE027CA269CD12EE;
// System.Action`2<System.String,Grpc.Core.SerializationContext>
struct Action_2_t7EEB0EFF03BAE4AFAA313496DFB6525080E5CE7D;
// System.Collections.Generic.Dictionary`2<System.Int32,System.Text.Encoding>
struct Dictionary_2_t87EDE08B2E48F793A22DE50D6B3CC2E7EBB2DB54;
// System.Collections.Generic.Dictionary`2<System.Int32,System.Threading.Tasks.Task>
struct Dictionary_2_t403063CE4960B4F46C688912237C6A27E550FF55;
// System.Collections.Generic.Dictionary`2<System.Object,System.Object>
struct Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA;
// System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>
struct Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50;
// System.Collections.Generic.Dictionary`2<System.String,System.Object>
struct Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710;
// System.Func`1<Grpc.Core.Metadata>
struct Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C;
// System.Func`1<System.Object>
struct Func_1_tD5C081AE11746B200C711DD48DBEB00E3A9276D4;
// System.Func`1<Grpc.Core.Status>
struct Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0;
// System.Func`1<System.Threading.Tasks.Task/ContingentProperties>
struct Func_1_tD59A12717D79BFB403BF973694B1BE5B85474BD1;
// System.Func`2<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>,System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty>>
struct Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A;
// System.Func`2<System.Byte[],System.Object>
struct Func_2_tEE2E132F8908935795FF1561B1E774E68C1BFEFF;
// System.Func`2<System.Byte[],System.String>
struct Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF;
// System.Func`2<Grpc.Core.DeserializationContext,System.String>
struct Func_2_t6ADD99E818374172130D81E77B19B28F7304525A;
// System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>
struct Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B;
// System.Func`2<System.Object,System.Collections.Generic.IEnumerable`1<System.Object>>
struct Func_2_t9F45EF9F857977243C345F24571962D2521DB4A1;
// System.Func`2<System.Object,System.Threading.Tasks.Task`1<Grpc.Core.Metadata>>
struct Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974;
// System.Func`2<System.Object,System.Byte[]>
struct Func_2_tAF06B745A36D23F105F8D17F16BC771E5D76E051;
// System.Func`2<System.Object,Grpc.Core.Metadata>
struct Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A;
// System.Func`2<System.Object,System.Object>
struct Func_2_tACBF5A1656250800CE861707354491F0611F6624;
// System.Func`2<System.Object,Grpc.Core.Status>
struct Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2;
// System.Func`2<System.String,System.Byte[]>
struct Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C;
// System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>
struct Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20;
// System.Func`4<System.Object,System.Object,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>
struct Func_4_tEF7A1074DE2C6C762CE8411E0D464E5C540B5300;
// System.Buffers.IBufferWriter`1<System.Byte>
struct IBufferWriter_1_t09B22D28ED83DD9237FC6D4E8014388593A8C897;
// System.Collections.Generic.IDictionary`2<System.Object,System.Object>
struct IDictionary_2_t823399AD16F88CDEB25958D8CB61771376A5ADED;
// System.Collections.Generic.IEnumerable`1<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>
struct IEnumerable_1_t11C12A50D936B815C9A52AD47F96DC9A846854A3;
// System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty>
struct IEnumerable_1_t1AFD9AEB5C922393BDE2F0B8D2EBD15D0625DD13;
// System.Collections.Generic.IEnumerable`1<Grpc.Core.CallCredentials>
struct IEnumerable_1_t5D905312B42AF316BF3D655B529862F13E3498D4;
// System.Collections.Generic.IEnumerable`1<Grpc.Core.Interceptors.Interceptor>
struct IEnumerable_1_t37CBE1A5D68AA0B27E8B20742699245769A385E0;
// System.Collections.Generic.IEnumerable`1<System.Object>
struct IEnumerable_1_tF95C9E01A913DD50575531C8305932628663D9E9;
// System.Collections.Generic.IEnumerable`1<Grpc.Core.Metadata/Entry>
struct IEnumerable_1_tEAC7DDD505B80C7E05BB576BBC1544B7EC7D107E;
// System.Collections.Generic.IEnumerator`1<Grpc.Core.Metadata/Entry>
struct IEnumerator_1_t3ACC894ED1ACD29FAE38464591017AC225A772D1;
// System.Collections.Generic.IEqualityComparer`1<System.Object>
struct IEqualityComparer_1_t2CA7720C7ADCCDECD3B02E45878B4478619D5347;
// System.Collections.Generic.IEqualityComparer`1<System.String>
struct IEqualityComparer_1_tAE94C8F24AD5B94D4EE85CA9FC59E3409D41CAF7;
// System.Collections.Generic.IList`1<System.Action`1<Grpc.Core.ServiceBinderBase>>
struct IList_1_tA5E14B21AED61EA1A6C9BD61B9A4A1506E70C7D8;
// System.Collections.Generic.IList`1<Grpc.Core.CallCredentials>
struct IList_1_t9704F5E56970F4A32E56E36604728492AE7A05BA;
// System.Collections.Generic.IReadOnlyList`1<System.Action`1<Grpc.Core.ServiceBinderBase>>
struct IReadOnlyList_1_t08196DC4F3B8F3A8306EDC2202B86B872B2A4A44;
// System.Collections.Generic.IReadOnlyList`1<Grpc.Core.CallCredentials>
struct IReadOnlyList_1_t163D19787906CD89531DDBE37C1945B11B946706;
// System.Collections.Generic.Dictionary`2/KeyCollection<System.Object,System.Object>
struct KeyCollection_tB45A861D090B15129521119AE48ED3813820A974;
// System.Collections.Generic.Dictionary`2/KeyCollection<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>
struct KeyCollection_tD6533BD09AABCB5ABC4125CC58804B9B4654E7CA;
// System.Collections.Generic.Dictionary`2/KeyCollection<System.String,System.Object>
struct KeyCollection_tE66790F09E854C19C7F612BEAD203AE626E90A36;
// System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>
struct List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5;
// System.Collections.Generic.List`1<Grpc.Core.AuthProperty>
struct List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A;
// System.Collections.Generic.List`1<Grpc.Core.CallCredentials>
struct List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1;
// System.Collections.Generic.List`1<System.Object>
struct List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D;
// System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>
struct List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337;
// Grpc.Core.Marshaller`1<System.Object>
struct Marshaller_1_t5AF2761BFFFBF2DF3FEBBAA0BEEE874BA4435EC4;
// Grpc.Core.Marshaller`1<System.String>
struct Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04;
// System.Predicate`1<System.Object>
struct Predicate_1_t8342C85FF4E41CD1F7024AC0CDC3E5312A32CB12;
// System.Predicate`1<System.Threading.Tasks.Task>
struct Predicate_1_t7F48518B008C1472339EEEBABA3DE203FE1F26ED;
// System.Collections.ObjectModel.ReadOnlyCollection`1<System.Action`1<Grpc.Core.ServiceBinderBase>>
struct ReadOnlyCollection_1_tBC605066ABB00B74ED2389020F64275AA494E462;
// System.Collections.ObjectModel.ReadOnlyCollection`1<Grpc.Core.CallCredentials>
struct ReadOnlyCollection_1_tC203378F06B8F145A48BFB0A1546E72C1B700445;
// System.Collections.ObjectModel.ReadOnlyCollection`1<System.Object>
struct ReadOnlyCollection_1_t5397DF0DB61D1090E7BBC89395CECB8D020CED92;
// System.Threading.Tasks.TaskFactory`1<Grpc.Core.Metadata>
struct TaskFactory_1_tDAEC71AE5F736E76304BA6958D048E7AE8FF6AA9;
// System.Threading.Tasks.Task`1<Grpc.Core.Metadata>
struct Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707;
// System.Threading.Tasks.Task`1<System.Threading.Tasks.VoidTaskResult>
struct Task_1_tE41CFF640EB7C045550D9D0D92BE67533B084C17;
// System.Collections.Generic.Dictionary`2/ValueCollection<System.Object,System.Object>
struct ValueCollection_t038245E04B5D2A80048D9F8021A23E69A0C9DBAA;
// System.Collections.Generic.Dictionary`2/ValueCollection<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>
struct ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010;
// System.Collections.Generic.Dictionary`2/ValueCollection<System.String,System.Object>
struct ValueCollection_tC9D91E8A3198E40EA339059703AB10DFC9F5CC2E;
// System.Action`1<Grpc.Core.ServiceBinderBase>[]
struct Action_1U5BU5D_tEEB96F533EF5D717115A455D9AB8389102E41757;
// System.Collections.Generic.Dictionary`2/Entry<System.Object,System.Object>[]
struct EntryU5BU5D_t1E85CBF91297C9D62A0FC2AD29FD24E33C8A5E54;
// System.Collections.Generic.Dictionary`2/Entry<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>[]
struct EntryU5BU5D_tD228B2E95758C15CE9D81276BE2C569EDA913031;
// System.Collections.Generic.Dictionary`2/Entry<System.String,System.Object>[]
struct EntryU5BU5D_t233BB24ED01E2D8D65B0651D54B8E3AD125CAF96;
// Grpc.Core.AuthProperty[]
struct AuthPropertyU5BU5D_t1B9314F35919A8B7A7BEA7BC89C18B4D3AA69A76;
// System.Byte[]
struct ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031;
// Grpc.Core.CallCredentials[]
struct CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD;
// System.Delegate[]
struct DelegateU5BU5D_tC5AB7E8F745616680F337909D3A8E6C722CDF771;
// System.Int32[]
struct Int32U5BU5D_t19C97395396A72ECAF310612F0760F165060314C;
// System.IntPtr[]
struct IntPtrU5BU5D_tFD177F8C806A6921AD7150264CCC62FA00CAD832;
// Grpc.Core.Interceptors.Interceptor[]
struct InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2;
// System.Object[]
struct ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918;
// System.Diagnostics.StackTrace[]
struct StackTraceU5BU5D_t32FBCB20930EAF5BAE3F450FF75228E5450DA0DF;
// System.Type[]
struct TypeU5BU5D_t97234E1129B564EB38B8D85CAC2AD8B5B9522FFB;
// Grpc.Core.Metadata/Entry[]
struct EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4;
// System.Action
struct Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07;
// System.ArgumentException
struct ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263;
// Grpc.Core.AsyncAuthInterceptor
struct AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968;
// System.AsyncCallback
struct AsyncCallback_t7FEF460CBDCFB9C5FA2EF776984778B9A4145F4C;
// System.Attribute
struct Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA;
// Grpc.Core.AuthContext
struct AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57;
// Grpc.Core.AuthInterceptorContext
struct AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D;
// Grpc.Core.AuthProperty
struct AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF;
// Grpc.Core.BindServiceMethodAttribute
struct BindServiceMethodAttribute_tA0C4802BE18E8C3D565943379450CB84CDEE2CDA;
// System.Reflection.Binder
struct Binder_t91BFCE95A7057FADF4D8A1A342AFE52872246235;
// Grpc.Core.CallCredentials
struct CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466;
// Grpc.Core.CallCredentialsConfiguratorBase
struct CallCredentialsConfiguratorBase_t90FECA680004ADF081D728A5765058B4643E0078;
// Grpc.Core.CallInvoker
struct CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF;
// Grpc.Core.CallOptions
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E;
// System.Threading.CancellationToken
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED;
// System.Threading.CancellationTokenSource
struct CancellationTokenSource_tAAE1E0033BCFC233801F8CB4CED5C852B350CB7B;
// Grpc.Core.ChannelBase
struct ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5;
// Grpc.Core.ChannelCredentials
struct ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730;
// Grpc.Core.ChannelCredentialsConfiguratorBase
struct ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E;
// Grpc.Core.ClientBase
struct ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391;
// System.Globalization.CodePageDataItem
struct CodePageDataItem_t52460FA30AE37F4F26ACB81055E58002262F19F2;
// System.Threading.ContextCallback
struct ContextCallback_tE8AFBDBFCC040FDA8DA8C1EEFE9BD66B16BDA007;
// Grpc.Core.ContextPropagationOptions
struct ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B;
// Grpc.Core.ContextPropagationToken
struct ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751;
// System.Text.DecoderFallback
struct DecoderFallback_t7324102215E4ED41EC065C02EB501CB0BC23CD90;
// System.Delegate
struct Delegate_t;
// System.DelegateData
struct DelegateData_t9B286B493293CD2D23A5B2B5EF0E5B1324C2B77E;
// Grpc.Core.DeserializationContext
struct DeserializationContext_t8C084AFEE582F438C6A9F8392DB3C4A92A930FAC;
// System.Diagnostics.CodeAnalysis.DynamicallyAccessedMembersAttribute
struct DynamicallyAccessedMembersAttribute_t2F26B97D513CC0C186D7433DD2935BDED77C481E;
// Microsoft.CodeAnalysis.EmbeddedAttribute
struct EmbeddedAttribute_t1EE07D5A21E2F81B6A651A3843A9BE8E5ADEE16D;
// System.Text.EncoderFallback
struct EncoderFallback_tD2C40CE114AA9D8E1F7196608B2D088548015293;
// System.Text.Encoding
struct Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095;
// System.Exception
struct Exception_t;
// System.IAsyncResult
struct IAsyncResult_t7B9B5A0ECB35DCEC31B8A8122C37D687369253B5;
// System.Runtime.CompilerServices.IAsyncStateMachine
struct IAsyncStateMachine_t0680C7F905C553076B552D5A1A6E39E2F0F36AA2;
// System.Collections.IDictionary
struct IDictionary_t6D03155AF1FA9083817AA5B6AD7DEEACC26AB220;
// System.Collections.IEnumerator
struct IEnumerator_t7B609C2FFA6EB5167D9C62A0C32A21DE2F666DAA;
// Grpc.Core.IMethod
struct IMethod_t96B5785EC279D946C87BF8B1008813D809B788FF;
// Grpc.Core.Interceptors.InterceptingCallInvoker
struct InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4;
// Grpc.Core.Interceptors.Interceptor
struct Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0;
// System.InvalidOperationException
struct InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB;
// System.Runtime.CompilerServices.IsReadOnlyAttribute
struct IsReadOnlyAttribute_t0A4E747F0A0347653B3EC39C15485282851A10B2;
// Grpc.Core.KeyCertificatePair
struct KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD;
// System.Reflection.MemberFilter
struct MemberFilter_tF644F1AE82F611B677CE1964D5A3277DDA21D553;
// Grpc.Core.Metadata
struct Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06;
// System.Reflection.MethodInfo
struct MethodInfo_t;
// System.NotImplementedException
struct NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8;
// System.NotSupportedException
struct NotSupportedException_t1429765983D409BD2986508963C98D214E4EBF4A;
// System.Runtime.CompilerServices.NullableAttribute
struct NullableAttribute_t7181E30ED50BDD358745EA005FCEC8C87AEF5EBA;
// System.Runtime.CompilerServices.NullableContextAttribute
struct NullableContextAttribute_t4A7A18FD00A42573002F14FA54763772B783B64C;
// Grpc.Core.RpcException
struct RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72;
// System.Runtime.Serialization.SafeSerializationManager
struct SafeSerializationManager_tCBB85B95DFD1634237140CD892E82D06ECB3F5E6;
// Grpc.Core.SerializationContext
struct SerializationContext_tB58A48F5BD333BD147B799711A88A26662D09ADB;
// Grpc.Core.ServerCallContext
struct ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9;
// Grpc.Core.ServerServiceDefinition
struct ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0;
// Grpc.Core.ServiceBinderBase
struct ServiceBinderBase_tFB0E641375E302B3D9DEC054D9D9BA8197FC104A;
// Grpc.Core.SslCredentials
struct SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3;
// System.Threading.Tasks.StackGuard
struct StackGuard_tACE063A1B7374BDF4AD472DE4585D05AD8745352;
// System.String
struct String_t;
// System.Threading.Tasks.Task
struct Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572;
// System.Threading.Tasks.TaskFactory
struct TaskFactory_tF781BD37BE23917412AD83424D1497C7C1509DF0;
// System.Threading.Tasks.TaskScheduler
struct TaskScheduler_t3F0550EBEF7C41F74EC8C08FF4BED0D8CE66006E;
// System.Type
struct Type_t;
// Grpc.Core.Internal.UnimplementedCallInvoker
struct UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435;
// Grpc.Core.VerifyPeerCallback
struct VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71;
// Grpc.Core.VerifyPeerContext
struct VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8;
// System.Void
struct Void_t4861ACF8F4594C3437BB48B6E56783494B843915;
// Grpc.Core.WriteOptions
struct WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050;
// Grpc.Core.AuthContext/<>c
struct U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677;
// Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials
struct AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862;
// Grpc.Core.CallCredentials/CompositeCallCredentials
struct CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E;
// Grpc.Core.Interceptors.CallInvokerExtensions/MetadataInterceptor
struct MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975;
// Grpc.Core.ChannelCredentials/CompositeChannelCredentials
struct CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740;
// Grpc.Core.ChannelCredentials/InsecureCredentials
struct InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A;
// Grpc.Core.ClientBase/ClientBaseConfiguration
struct ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D;
// Grpc.Core.Metadata/<GetAll>d__11
struct U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3;
// Grpc.Core.Metadata/Entry
struct Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB;
// Grpc.Core.ServerServiceDefinition/Builder
struct Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32;
// System.Threading.Tasks.Task/ContingentProperties
struct ContingentProperties_t3FA59480914505CEA917B1002EC675F29D0CB540;
// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInterceptor
struct ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425;

IL2CPP_EXTERN_C RuntimeClass* Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Exception_t_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* IDisposable_t030E0496B4E0E4E4F086825007979AF51F7248C5_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* IEnumerable_1_t37CBE1A5D68AA0B27E8B20742699245769A385E0_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* IEnumerable_1_t7073586B577B76AB9907E9E8F21F45C2C364BC4F_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* IEnumerator_1_t5F4BEB00470960A0E76DDE9DAA1DF5168331CB41_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* IEnumerator_1_tF7846B70B58A638138E3AFE67ED47A962B32B2E1_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* IEnumerator_t7B609C2FFA6EB5167D9C62A0C32A21DE2F666DAA_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Il2CppComObject_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Marshal_tD976A56A90263C3CE2B780D4B1CADADE2E70B4A7_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* NotSupportedException_t1429765983D409BD2986508963C98D214E4EBF4A_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* StatusCode_t96CF7C2EFE67B9B0DCEF94E8B24F704141287375_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C RuntimeClass* WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_il2cpp_TypeInfo_var;
IL2CPP_EXTERN_C String_t* _stringLiteral0054B3392632CC246C5CE404303EC971B31E150D;
IL2CPP_EXTERN_C String_t* _stringLiteral02E811106795854EAA599A5506DD0042594581C0;
IL2CPP_EXTERN_C String_t* _stringLiteral0D292B6278EB6FF9C4EA2C48A1C5CE6FB09E1678;
IL2CPP_EXTERN_C String_t* _stringLiteral29D01ED29B3E08D159739A60EE14FFC25AC4F8FE;
IL2CPP_EXTERN_C String_t* _stringLiteral2EC6DE2D71C83B236D586235EA0A7A8E7FF32E9F;
IL2CPP_EXTERN_C String_t* _stringLiteral320772EF40302B49A179DB96BAD02224E97B4018;
IL2CPP_EXTERN_C String_t* _stringLiteral330132604A8C7C73B4775BFE93C8FBEC4857F022;
IL2CPP_EXTERN_C String_t* _stringLiteral46F273EF641E07D271D91E0DC24A4392582671F8;
IL2CPP_EXTERN_C String_t* _stringLiteral61B4338BE86CB9213EE71FAD62B393B8922755E8;
IL2CPP_EXTERN_C String_t* _stringLiteral7C19A3E78437A0AD89B0D16EBF9C74FEDCCBB8AC;
IL2CPP_EXTERN_C String_t* _stringLiteral822EDE75F5970A676E660AB56E0A62A92F681766;
IL2CPP_EXTERN_C String_t* _stringLiteral8E8AE59D86EF9B6862311A1970B81D68866956AB;
IL2CPP_EXTERN_C String_t* _stringLiteral92ACAEB1D158C2D3AC9BB0C86CA905A5314FBBDC;
IL2CPP_EXTERN_C String_t* _stringLiteralA00F3DF646A4521C1DC3F40012285F4CA6DB6A43;
IL2CPP_EXTERN_C String_t* _stringLiteralCC5275F31A11D4AC091381249B0215D81AB03CF9;
IL2CPP_EXTERN_C String_t* _stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709;
IL2CPP_EXTERN_C String_t* _stringLiteralE064A593217F8D9C315BF354EAAD770AB3C8246C;
IL2CPP_EXTERN_C String_t* _stringLiteralE33CCB09E2FF8EF27D0D937E4C19A6936680A454;
IL2CPP_EXTERN_C String_t* _stringLiteralE705C6345C26AF82E64D22DBE44B2A3514F2F06F;
IL2CPP_EXTERN_C String_t* _stringLiteralE7D028CCE3B6E7B61AE2C752D7AE970DA04AB7C6;
IL2CPP_EXTERN_C String_t* _stringLiteralE8BC822461C381F0D1DE9430568E970322E2D07A;
IL2CPP_EXTERN_C String_t* _stringLiteralEFDA3D70B0D50B492DB756FABC3B4A4E00B0F8AF;
IL2CPP_EXTERN_C String_t* _stringLiteralFF5D592BA64F7E8E842A9A677126A31DF0EEBCDE;
IL2CPP_EXTERN_C const RuntimeMethod* AsyncTaskMethodBuilder_Start_TisU3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44_m9121BD027CA131910ADCDDCFA9EBE759B4E9AD46_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* ClientBaseConfiguration_U3CCreateDecoratedCallInvokerU3Eb__5_0_m6D33050551D69C47894B758AB866E63FDE9F2FD8_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* CompositeChannelCredentials__ctor_mECB83F30B1E805ACD5F037105A1C793077ED0E86_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* DeserializationContext_PayloadAsNewBuffer_m90E78A24BD2E0B2AB24095E4C405D6610D51D6D0_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* DeserializationContext_PayloadAsReadOnlySequence_m18B455D5E3F552FA7055CDD31AD32151DCFB1441_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Dictionary_2_TryGetValue_m06D597E171BEBB18AC362AE27BBD7BEE0A0C4F7F_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Dictionary_2__ctor_mC4F3DF292BAD88F4BF193C49CD689FAEBC4570A9_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Dictionary_2_get_Item_mCC86C07EAC13D2EC7C63C99AB267F4A9B0BCBA7E_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Dictionary_2_get_Values_m23DB66E8602DED9744E45784C6E3A2F2C8F513D4_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Enumerable_Reverse_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m1C1BF18516E49D40D7DBDB84100A0B8168E2B4BA_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Enumerable_SelectMany_TisList_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mEA8E736006D5A38482763344EB5EF9B911A7DDFA_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckArgument_m2B157D8C7C01045ECCABED1F0A51A889C9759D34_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisAsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968_m0512535AE0D48827C37B1D3A08E71D0AE57FEE20_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_m2BEB1EC4BC44E79026DB6011E1E94018C77954EE_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisCallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466_mB0D2D758B2EE1819303789F98C090EA881FE012E_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_mAD4DE615EDB070D206521A1845F088B4ABAC8966_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_m13ACE0719712FCF60B60D0CFFBE35BAC3F9E1DB4_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_m7EB6EB36D0C253043670DC0A19000775E31CB16C_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisDictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50_m7ED3349BDCE75D560CFB3D131E416BFB92D48222_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisFunc_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B_m5E04E5E300A4C3AD62F603401B642AB7C7656305_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisFunc_4_t737C862B437880FF1CD553B949328C8BCF76DD20_m5CD271FF760C0D77688A24811BE9FF6518D3090D_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisInterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2_m54DCBCEF26FDD11320936B2E59AE28F2E7CAC4CC_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m4E082761E5C0C7589B1EA55BB46E22E68FDA6B54_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisMetadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_m3DDAE807CB04B4ADAC6FB166B7EC34BE2A3720AE_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckState_m8B3B2F0B0AE8E3A42D3252666E1E51C402CE81E9_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* GrpcPreconditions_CheckState_mCDE04614CE69128AF99EE030B5F83D386FDEC375_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_Add_m8626F0812751DA3B7FF22FC178565F1326A68CD0_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_AsReadOnly_m730830764F278CFE6CD6A5E39836BFD8BED58AED_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_AsReadOnly_mF5D40FE5DCA9ABF87F4D97EB90234195022B9440_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_Clear_mAAD225470B9D453E533818038DA07BC1494513DC_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_Contains_m3750DD90BC38CC246D898710BA06F7F266DEFB78_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_CopyTo_m6CA081F6251F594F64C73BCB3B6915259FB42903_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_IndexOf_m098D47D60BAF4C30BB0E8FF3CF8EF49093D6FD2B_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_Insert_m383E4FA9E7D234C2BFAC3B41E038B15CFA80AF05_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_RemoveAt_m76954D81D216842D8A3ADD420E6588EABFA34A6A_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_Remove_mBE506315950A1EA3F5351D3E7A9216015D68A6FA_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1__ctor_m3339F822AD7AB85E44A3CE0FD82074C27CBB7903_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1__ctor_m5488D09159AC2D56E9F9779F44ADF0DD0E684049_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1__ctor_mC61673FF37950228AD28C7E2D5798F7E822165FE_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* List_1_set_Item_mDBC1AAC216D70CE8B5E8CB3AAF3083E65247CFC3_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Marshaller_1__ctor_mF20CB78867ABA38F189F7DCE7D6DCA50C197A6DE_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* Nullable_1__ctor_mB17304720EA19F5469A4883827F53A75FEB492CF_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* SerializationContext_Complete_m3C0A349F0B19FE856B4EFB5C7E7569D74025C330_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* SerializationContext_Complete_m5A51FF9AD59C9E706ACCC9FDD7BAF5E747709B9E_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* SerializationContext_GetBufferWriter_m5CAC81A9C524E847470E05C14D5359E7804EF265_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* U3CGetAllU3Ed__11_System_Collections_IEnumerator_Reset_mC30E2AFCFE8F4319C587AE4B69E7C8DAE429D37D_RuntimeMethod_var;
IL2CPP_EXTERN_C const RuntimeMethod* U3CU3Ec_U3Cget_PropertiesU3Eb__10_0_m0F85D3F37CE6E6128A2F35657DFC2B85FC2EC928_RuntimeMethod_var;
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E;;
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com;
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com;;
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke;
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke;;
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED;;
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_com;
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_com;;
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_pinvoke;
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_pinvoke;;
struct Delegate_t_marshaled_com;
struct Delegate_t_marshaled_pinvoke;
struct Exception_t;;
struct Exception_t_marshaled_com;
struct Exception_t_marshaled_com;;
struct Exception_t_marshaled_pinvoke;
struct Exception_t_marshaled_pinvoke;;

struct ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031;
struct CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD;
struct DelegateU5BU5D_tC5AB7E8F745616680F337909D3A8E6C722CDF771;
struct InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2;
struct ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918;
struct EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4;

IL2CPP_EXTERN_C_BEGIN
IL2CPP_EXTERN_C_END

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif

// <Module>
struct U3CModuleU3E_tC49EAFA6ECB6AB7A37F65A04B452D3D7802818B6 
{
};

// System.Collections.Generic.Dictionary`2<System.Object,System.Object>
struct Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA  : public RuntimeObject
{
	// System.Int32[] System.Collections.Generic.Dictionary`2::_buckets
	Int32U5BU5D_t19C97395396A72ECAF310612F0760F165060314C* ____buckets_0;
	// System.Collections.Generic.Dictionary`2/Entry<TKey,TValue>[] System.Collections.Generic.Dictionary`2::_entries
	EntryU5BU5D_t1E85CBF91297C9D62A0FC2AD29FD24E33C8A5E54* ____entries_1;
	// System.Int32 System.Collections.Generic.Dictionary`2::_count
	int32_t ____count_2;
	// System.Int32 System.Collections.Generic.Dictionary`2::_freeList
	int32_t ____freeList_3;
	// System.Int32 System.Collections.Generic.Dictionary`2::_freeCount
	int32_t ____freeCount_4;
	// System.Int32 System.Collections.Generic.Dictionary`2::_version
	int32_t ____version_5;
	// System.Collections.Generic.IEqualityComparer`1<TKey> System.Collections.Generic.Dictionary`2::_comparer
	RuntimeObject* ____comparer_6;
	// System.Collections.Generic.Dictionary`2/KeyCollection<TKey,TValue> System.Collections.Generic.Dictionary`2::_keys
	KeyCollection_tB45A861D090B15129521119AE48ED3813820A974* ____keys_7;
	// System.Collections.Generic.Dictionary`2/ValueCollection<TKey,TValue> System.Collections.Generic.Dictionary`2::_values
	ValueCollection_t038245E04B5D2A80048D9F8021A23E69A0C9DBAA* ____values_8;
	// System.Object System.Collections.Generic.Dictionary`2::_syncRoot
	RuntimeObject* ____syncRoot_9;
};

// System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>
struct Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50  : public RuntimeObject
{
	// System.Int32[] System.Collections.Generic.Dictionary`2::_buckets
	Int32U5BU5D_t19C97395396A72ECAF310612F0760F165060314C* ____buckets_0;
	// System.Collections.Generic.Dictionary`2/Entry<TKey,TValue>[] System.Collections.Generic.Dictionary`2::_entries
	EntryU5BU5D_tD228B2E95758C15CE9D81276BE2C569EDA913031* ____entries_1;
	// System.Int32 System.Collections.Generic.Dictionary`2::_count
	int32_t ____count_2;
	// System.Int32 System.Collections.Generic.Dictionary`2::_freeList
	int32_t ____freeList_3;
	// System.Int32 System.Collections.Generic.Dictionary`2::_freeCount
	int32_t ____freeCount_4;
	// System.Int32 System.Collections.Generic.Dictionary`2::_version
	int32_t ____version_5;
	// System.Collections.Generic.IEqualityComparer`1<TKey> System.Collections.Generic.Dictionary`2::_comparer
	RuntimeObject* ____comparer_6;
	// System.Collections.Generic.Dictionary`2/KeyCollection<TKey,TValue> System.Collections.Generic.Dictionary`2::_keys
	KeyCollection_tD6533BD09AABCB5ABC4125CC58804B9B4654E7CA* ____keys_7;
	// System.Collections.Generic.Dictionary`2/ValueCollection<TKey,TValue> System.Collections.Generic.Dictionary`2::_values
	ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010* ____values_8;
	// System.Object System.Collections.Generic.Dictionary`2::_syncRoot
	RuntimeObject* ____syncRoot_9;
};

// System.Collections.Generic.Dictionary`2<System.String,System.Object>
struct Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710  : public RuntimeObject
{
	// System.Int32[] System.Collections.Generic.Dictionary`2::_buckets
	Int32U5BU5D_t19C97395396A72ECAF310612F0760F165060314C* ____buckets_0;
	// System.Collections.Generic.Dictionary`2/Entry<TKey,TValue>[] System.Collections.Generic.Dictionary`2::_entries
	EntryU5BU5D_t233BB24ED01E2D8D65B0651D54B8E3AD125CAF96* ____entries_1;
	// System.Int32 System.Collections.Generic.Dictionary`2::_count
	int32_t ____count_2;
	// System.Int32 System.Collections.Generic.Dictionary`2::_freeList
	int32_t ____freeList_3;
	// System.Int32 System.Collections.Generic.Dictionary`2::_freeCount
	int32_t ____freeCount_4;
	// System.Int32 System.Collections.Generic.Dictionary`2::_version
	int32_t ____version_5;
	// System.Collections.Generic.IEqualityComparer`1<TKey> System.Collections.Generic.Dictionary`2::_comparer
	RuntimeObject* ____comparer_6;
	// System.Collections.Generic.Dictionary`2/KeyCollection<TKey,TValue> System.Collections.Generic.Dictionary`2::_keys
	KeyCollection_tE66790F09E854C19C7F612BEAD203AE626E90A36* ____keys_7;
	// System.Collections.Generic.Dictionary`2/ValueCollection<TKey,TValue> System.Collections.Generic.Dictionary`2::_values
	ValueCollection_tC9D91E8A3198E40EA339059703AB10DFC9F5CC2E* ____values_8;
	// System.Object System.Collections.Generic.Dictionary`2::_syncRoot
	RuntimeObject* ____syncRoot_9;
};

// System.Linq.EmptyEnumerable`1<System.Object>
struct EmptyEnumerable_1_t8C8873EF4F89FB0F86D91BA5B4D640E3A23AD28E  : public RuntimeObject
{
};

// System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>
struct List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5  : public RuntimeObject
{
	// T[] System.Collections.Generic.List`1::_items
	Action_1U5BU5D_tEEB96F533EF5D717115A455D9AB8389102E41757* ____items_1;
	// System.Int32 System.Collections.Generic.List`1::_size
	int32_t ____size_2;
	// System.Int32 System.Collections.Generic.List`1::_version
	int32_t ____version_3;
	// System.Object System.Collections.Generic.List`1::_syncRoot
	RuntimeObject* ____syncRoot_4;
};

// System.Collections.Generic.List`1<Grpc.Core.AuthProperty>
struct List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A  : public RuntimeObject
{
	// T[] System.Collections.Generic.List`1::_items
	AuthPropertyU5BU5D_t1B9314F35919A8B7A7BEA7BC89C18B4D3AA69A76* ____items_1;
	// System.Int32 System.Collections.Generic.List`1::_size
	int32_t ____size_2;
	// System.Int32 System.Collections.Generic.List`1::_version
	int32_t ____version_3;
	// System.Object System.Collections.Generic.List`1::_syncRoot
	RuntimeObject* ____syncRoot_4;
};

// System.Collections.Generic.List`1<Grpc.Core.CallCredentials>
struct List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1  : public RuntimeObject
{
	// T[] System.Collections.Generic.List`1::_items
	CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* ____items_1;
	// System.Int32 System.Collections.Generic.List`1::_size
	int32_t ____size_2;
	// System.Int32 System.Collections.Generic.List`1::_version
	int32_t ____version_3;
	// System.Object System.Collections.Generic.List`1::_syncRoot
	RuntimeObject* ____syncRoot_4;
};

// System.Collections.Generic.List`1<System.Object>
struct List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D  : public RuntimeObject
{
	// T[] System.Collections.Generic.List`1::_items
	ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* ____items_1;
	// System.Int32 System.Collections.Generic.List`1::_size
	int32_t ____size_2;
	// System.Int32 System.Collections.Generic.List`1::_version
	int32_t ____version_3;
	// System.Object System.Collections.Generic.List`1::_syncRoot
	RuntimeObject* ____syncRoot_4;
};

// System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>
struct List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337  : public RuntimeObject
{
	// T[] System.Collections.Generic.List`1::_items
	EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4* ____items_1;
	// System.Int32 System.Collections.Generic.List`1::_size
	int32_t ____size_2;
	// System.Int32 System.Collections.Generic.List`1::_version
	int32_t ____version_3;
	// System.Object System.Collections.Generic.List`1::_syncRoot
	RuntimeObject* ____syncRoot_4;
};

// Grpc.Core.Marshaller`1<System.String>
struct Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04  : public RuntimeObject
{
	// System.Func`2<T,System.Byte[]> Grpc.Core.Marshaller`1::serializer
	Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C* ___serializer_0;
	// System.Func`2<System.Byte[],T> Grpc.Core.Marshaller`1::deserializer
	Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF* ___deserializer_1;
	// System.Action`2<T,Grpc.Core.SerializationContext> Grpc.Core.Marshaller`1::contextualSerializer
	Action_2_t7EEB0EFF03BAE4AFAA313496DFB6525080E5CE7D* ___contextualSerializer_2;
	// System.Func`2<Grpc.Core.DeserializationContext,T> Grpc.Core.Marshaller`1::contextualDeserializer
	Func_2_t6ADD99E818374172130D81E77B19B28F7304525A* ___contextualDeserializer_3;
};

// System.Collections.ObjectModel.ReadOnlyCollection`1<System.Action`1<Grpc.Core.ServiceBinderBase>>
struct ReadOnlyCollection_1_tBC605066ABB00B74ED2389020F64275AA494E462  : public RuntimeObject
{
	// System.Collections.Generic.IList`1<T> System.Collections.ObjectModel.ReadOnlyCollection`1::list
	RuntimeObject* ___list_0;
	// System.Object System.Collections.ObjectModel.ReadOnlyCollection`1::_syncRoot
	RuntimeObject* ____syncRoot_1;
};

// System.Collections.ObjectModel.ReadOnlyCollection`1<Grpc.Core.CallCredentials>
struct ReadOnlyCollection_1_tC203378F06B8F145A48BFB0A1546E72C1B700445  : public RuntimeObject
{
	// System.Collections.Generic.IList`1<T> System.Collections.ObjectModel.ReadOnlyCollection`1::list
	RuntimeObject* ___list_0;
	// System.Object System.Collections.ObjectModel.ReadOnlyCollection`1::_syncRoot
	RuntimeObject* ____syncRoot_1;
};

// System.Collections.Generic.Dictionary`2/ValueCollection<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>
struct ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010  : public RuntimeObject
{
	// System.Collections.Generic.Dictionary`2<TKey,TValue> System.Collections.Generic.Dictionary`2/ValueCollection::_dictionary
	Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* ____dictionary_0;
};

// Grpc.Core.AsyncStreamReaderExtensions
struct AsyncStreamReaderExtensions_t9FEE303E0031CC2E0186A1EACB23F500239DEDD2  : public RuntimeObject
{
};

// System.Attribute
struct Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA  : public RuntimeObject
{
};

// Grpc.Core.AuthContext
struct AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57  : public RuntimeObject
{
	// System.String Grpc.Core.AuthContext::peerIdentityPropertyName
	String_t* ___peerIdentityPropertyName_0;
	// System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>> Grpc.Core.AuthContext::properties
	Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* ___properties_1;
};

// Grpc.Core.AuthInterceptorContext
struct AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D  : public RuntimeObject
{
	// System.String Grpc.Core.AuthInterceptorContext::serviceUrl
	String_t* ___serviceUrl_0;
	// System.String Grpc.Core.AuthInterceptorContext::methodName
	String_t* ___methodName_1;
};

// Grpc.Core.AuthProperty
struct AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF  : public RuntimeObject
{
	// System.String Grpc.Core.AuthProperty::name
	String_t* ___name_1;
	// System.Byte[] Grpc.Core.AuthProperty::valueBytes
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___valueBytes_2;
	// System.String Grpc.Core.AuthProperty::lazyValue
	String_t* ___lazyValue_3;
};

// Grpc.Core.CallCredentials
struct CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466  : public RuntimeObject
{
};

// Grpc.Core.CallCredentialsConfiguratorBase
struct CallCredentialsConfiguratorBase_t90FECA680004ADF081D728A5765058B4643E0078  : public RuntimeObject
{
};

// Grpc.Core.CallInvoker
struct CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF  : public RuntimeObject
{
};

// Grpc.Core.Interceptors.CallInvokerExtensions
struct CallInvokerExtensions_t1825E5DDA80A68B43B9BE18CCBE0E959E4646D15  : public RuntimeObject
{
};

// Grpc.Core.ChannelBase
struct ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5  : public RuntimeObject
{
	// System.String Grpc.Core.ChannelBase::target
	String_t* ___target_0;
};

// Grpc.Core.ChannelCredentials
struct ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730  : public RuntimeObject
{
};

// Grpc.Core.ChannelCredentialsConfiguratorBase
struct ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E  : public RuntimeObject
{
};

// Grpc.Core.Interceptors.ChannelExtensions
struct ChannelExtensions_t46723C2BE8C2D67C7A92182BF9D50A3B3821659B  : public RuntimeObject
{
};

// Grpc.Core.ClientBase
struct ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391  : public RuntimeObject
{
	// Grpc.Core.ClientBase/ClientBaseConfiguration Grpc.Core.ClientBase::configuration
	ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* ___configuration_0;
	// Grpc.Core.CallInvoker Grpc.Core.ClientBase::callInvoker
	CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___callInvoker_1;
};

// Grpc.Core.ContextPropagationOptions
struct ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B  : public RuntimeObject
{
	// System.Boolean Grpc.Core.ContextPropagationOptions::propagateDeadline
	bool ___propagateDeadline_1;
	// System.Boolean Grpc.Core.ContextPropagationOptions::propagateCancellation
	bool ___propagateCancellation_2;
};

// Grpc.Core.ContextPropagationToken
struct ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751  : public RuntimeObject
{
};

// Grpc.Core.DeserializationContext
struct DeserializationContext_t8C084AFEE582F438C6A9F8392DB3C4A92A930FAC  : public RuntimeObject
{
};

// System.Text.Encoding
struct Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095  : public RuntimeObject
{
	// System.Int32 System.Text.Encoding::m_codePage
	int32_t ___m_codePage_9;
	// System.Globalization.CodePageDataItem System.Text.Encoding::dataItem
	CodePageDataItem_t52460FA30AE37F4F26ACB81055E58002262F19F2* ___dataItem_10;
	// System.Boolean System.Text.Encoding::m_deserializedFromEverett
	bool ___m_deserializedFromEverett_11;
	// System.Boolean System.Text.Encoding::m_isReadOnly
	bool ___m_isReadOnly_12;
	// System.Text.EncoderFallback System.Text.Encoding::encoderFallback
	EncoderFallback_tD2C40CE114AA9D8E1F7196608B2D088548015293* ___encoderFallback_13;
	// System.Text.DecoderFallback System.Text.Encoding::decoderFallback
	DecoderFallback_t7324102215E4ED41EC065C02EB501CB0BC23CD90* ___decoderFallback_14;
};

// Grpc.Core.Api.Utils.EncodingExtensions
struct EncodingExtensions_t905D79658C5E5203B11BB35CBB23EB2309F77DAE  : public RuntimeObject
{
};

// Grpc.Core.Utils.GrpcPreconditions
struct GrpcPreconditions_t305728804F78FEBF22B466B4383E71111A1703A9  : public RuntimeObject
{
};

// Grpc.Core.Interceptors.Interceptor
struct Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0  : public RuntimeObject
{
};

// Grpc.Core.KeyCertificatePair
struct KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD  : public RuntimeObject
{
	// System.String Grpc.Core.KeyCertificatePair::certificateChain
	String_t* ___certificateChain_0;
	// System.String Grpc.Core.KeyCertificatePair::privateKey
	String_t* ___privateKey_1;
};

// Grpc.Core.Marshallers
struct Marshallers_tCA61DDFDB68891E366B7B0A03C492CA0D5AB6F2D  : public RuntimeObject
{
};

// System.Reflection.MemberInfo
struct MemberInfo_t  : public RuntimeObject
{
};

// Grpc.Core.Metadata
struct Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06  : public RuntimeObject
{
	// System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry> Grpc.Core.Metadata::entries
	List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* ___entries_4;
	// System.Boolean Grpc.Core.Metadata::readOnly
	bool ___readOnly_5;
};

// Grpc.Core.SerializationContext
struct SerializationContext_tB58A48F5BD333BD147B799711A88A26662D09ADB  : public RuntimeObject
{
};

// Grpc.Core.ServerCallContext
struct ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9  : public RuntimeObject
{
	// System.Collections.Generic.Dictionary`2<System.Object,System.Object> Grpc.Core.ServerCallContext::userState
	Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* ___userState_0;
};

// Grpc.Core.ServerServiceDefinition
struct ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0  : public RuntimeObject
{
	// System.Collections.Generic.IReadOnlyList`1<System.Action`1<Grpc.Core.ServiceBinderBase>> Grpc.Core.ServerServiceDefinition::addMethodActions
	RuntimeObject* ___addMethodActions_0;
};

// Grpc.Core.ServiceBinderBase
struct ServiceBinderBase_tFB0E641375E302B3D9DEC054D9D9BA8197FC104A  : public RuntimeObject
{
};

// System.String
struct String_t  : public RuntimeObject
{
	// System.Int32 System.String::_stringLength
	int32_t ____stringLength_4;
	// System.Char System.String::_firstChar
	Il2CppChar ____firstChar_5;
};

// System.Threading.Tasks.Task
struct Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572  : public RuntimeObject
{
	// System.Int32 modreq(System.Runtime.CompilerServices.IsVolatile) System.Threading.Tasks.Task::m_taskId
	int32_t ___m_taskId_1;
	// System.Delegate System.Threading.Tasks.Task::m_action
	Delegate_t* ___m_action_2;
	// System.Object System.Threading.Tasks.Task::m_stateObject
	RuntimeObject* ___m_stateObject_3;
	// System.Threading.Tasks.TaskScheduler System.Threading.Tasks.Task::m_taskScheduler
	TaskScheduler_t3F0550EBEF7C41F74EC8C08FF4BED0D8CE66006E* ___m_taskScheduler_4;
	// System.Threading.Tasks.Task System.Threading.Tasks.Task::m_parent
	Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* ___m_parent_5;
	// System.Int32 modreq(System.Runtime.CompilerServices.IsVolatile) System.Threading.Tasks.Task::m_stateFlags
	int32_t ___m_stateFlags_6;
	// System.Object modreq(System.Runtime.CompilerServices.IsVolatile) System.Threading.Tasks.Task::m_continuationObject
	RuntimeObject* ___m_continuationObject_23;
	// System.Threading.Tasks.Task/ContingentProperties modreq(System.Runtime.CompilerServices.IsVolatile) System.Threading.Tasks.Task::m_contingentProperties
	ContingentProperties_t3FA59480914505CEA917B1002EC675F29D0CB540* ___m_contingentProperties_26;
};

// System.ValueType
struct ValueType_t6D9B272BD21782F0A9A14F2E41F85A50E97A986F  : public RuntimeObject
{
};
// Native definition for P/Invoke marshalling of System.ValueType
struct ValueType_t6D9B272BD21782F0A9A14F2E41F85A50E97A986F_marshaled_pinvoke
{
};
// Native definition for COM marshalling of System.ValueType
struct ValueType_t6D9B272BD21782F0A9A14F2E41F85A50E97A986F_marshaled_com
{
};

// Grpc.Core.VerifyPeerContext
struct VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8  : public RuntimeObject
{
	// System.String Grpc.Core.VerifyPeerContext::<TargetName>k__BackingField
	String_t* ___U3CTargetNameU3Ek__BackingField_0;
	// System.String Grpc.Core.VerifyPeerContext::<PeerPem>k__BackingField
	String_t* ___U3CPeerPemU3Ek__BackingField_1;
};

// Grpc.Core.VersionInfo
struct VersionInfo_t66A1A7AD28BC0A862737A6F37F018D854AFDEB76  : public RuntimeObject
{
};

// Grpc.Core.WriteOptions
struct WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050  : public RuntimeObject
{
	// Grpc.Core.WriteFlags Grpc.Core.WriteOptions::flags
	int32_t ___flags_1;
};

// Grpc.Core.AuthContext/<>c
struct U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677  : public RuntimeObject
{
};

// Grpc.Core.ClientBase/ClientBaseConfiguration
struct ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D  : public RuntimeObject
{
	// Grpc.Core.CallInvoker Grpc.Core.ClientBase/ClientBaseConfiguration::undecoratedCallInvoker
	CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___undecoratedCallInvoker_0;
	// System.String Grpc.Core.ClientBase/ClientBaseConfiguration::host
	String_t* ___host_1;
};

// Grpc.Core.Metadata/<GetAll>d__11
struct U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3  : public RuntimeObject
{
	// System.Int32 Grpc.Core.Metadata/<GetAll>d__11::<>1__state
	int32_t ___U3CU3E1__state_0;
	// Grpc.Core.Metadata/Entry Grpc.Core.Metadata/<GetAll>d__11::<>2__current
	Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___U3CU3E2__current_1;
	// System.Int32 Grpc.Core.Metadata/<GetAll>d__11::<>l__initialThreadId
	int32_t ___U3CU3El__initialThreadId_2;
	// Grpc.Core.Metadata Grpc.Core.Metadata/<GetAll>d__11::<>4__this
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___U3CU3E4__this_3;
	// System.String Grpc.Core.Metadata/<GetAll>d__11::key
	String_t* ___key_4;
	// System.String Grpc.Core.Metadata/<GetAll>d__11::<>3__key
	String_t* ___U3CU3E3__key_5;
	// System.Int32 Grpc.Core.Metadata/<GetAll>d__11::<i>5__2
	int32_t ___U3CiU3E5__2_6;
};

// Grpc.Core.Metadata/Entry
struct Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB  : public RuntimeObject
{
	// System.String Grpc.Core.Metadata/Entry::key
	String_t* ___key_0;
	// System.String Grpc.Core.Metadata/Entry::value
	String_t* ___value_1;
	// System.Byte[] Grpc.Core.Metadata/Entry::valueBytes
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___valueBytes_2;
};

// Grpc.Core.ServerServiceDefinition/Builder
struct Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32  : public RuntimeObject
{
	// System.Collections.Generic.Dictionary`2<System.String,System.Object> Grpc.Core.ServerServiceDefinition/Builder::duplicateDetector
	Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710* ___duplicateDetector_0;
	// System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>> Grpc.Core.ServerServiceDefinition/Builder::addMethodActions
	List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* ___addMethodActions_1;
};

// System.Collections.Generic.List`1/Enumerator<System.Object>
struct Enumerator_t9473BAB568A27E2339D48C1F91319E0F6D244D7A 
{
	// System.Collections.Generic.List`1<T> System.Collections.Generic.List`1/Enumerator::_list
	List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* ____list_0;
	// System.Int32 System.Collections.Generic.List`1/Enumerator::_index
	int32_t ____index_1;
	// System.Int32 System.Collections.Generic.List`1/Enumerator::_version
	int32_t ____version_2;
	// T System.Collections.Generic.List`1/Enumerator::_current
	RuntimeObject* ____current_3;
};

// System.Collections.Generic.List`1/Enumerator<Grpc.Core.Metadata/Entry>
struct Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6 
{
	// System.Collections.Generic.List`1<T> System.Collections.Generic.List`1/Enumerator::_list
	List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* ____list_0;
	// System.Int32 System.Collections.Generic.List`1/Enumerator::_index
	int32_t ____index_1;
	// System.Int32 System.Collections.Generic.List`1/Enumerator::_version
	int32_t ____version_2;
	// T System.Collections.Generic.List`1/Enumerator::_current
	Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ____current_3;
};

// System.Buffers.ReadOnlySequence`1<System.Byte>
struct ReadOnlySequence_1_t85097489C00952FC58DF9F3ACE926EAE9A51898A 
{
	// System.Object System.Buffers.ReadOnlySequence`1::_startObject
	RuntimeObject* ____startObject_0;
	// System.Object System.Buffers.ReadOnlySequence`1::_endObject
	RuntimeObject* ____endObject_1;
	// System.Int32 System.Buffers.ReadOnlySequence`1::_startInteger
	int32_t ____startInteger_2;
	// System.Int32 System.Buffers.ReadOnlySequence`1::_endInteger
	int32_t ____endInteger_3;
};

// System.Threading.Tasks.Task`1<Grpc.Core.Metadata>
struct Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707  : public Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572
{
	// TResult System.Threading.Tasks.Task`1::m_result
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___m_result_38;
};

// Grpc.Core.AsyncCallState
struct AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920 
{
	// System.Object Grpc.Core.AsyncCallState::responseHeadersAsync
	RuntimeObject* ___responseHeadersAsync_0;
	// System.Object Grpc.Core.AsyncCallState::getStatusFunc
	RuntimeObject* ___getStatusFunc_1;
	// System.Object Grpc.Core.AsyncCallState::getTrailersFunc
	RuntimeObject* ___getTrailersFunc_2;
	// System.Object Grpc.Core.AsyncCallState::disposeAction
	RuntimeObject* ___disposeAction_3;
	// System.Object Grpc.Core.AsyncCallState::callbackState
	RuntimeObject* ___callbackState_4;
};
// Native definition for P/Invoke marshalling of Grpc.Core.AsyncCallState
struct AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_pinvoke
{
	Il2CppIUnknown* ___responseHeadersAsync_0;
	Il2CppIUnknown* ___getStatusFunc_1;
	Il2CppIUnknown* ___getTrailersFunc_2;
	Il2CppIUnknown* ___disposeAction_3;
	Il2CppIUnknown* ___callbackState_4;
};
// Native definition for COM marshalling of Grpc.Core.AsyncCallState
struct AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_com
{
	Il2CppIUnknown* ___responseHeadersAsync_0;
	Il2CppIUnknown* ___getStatusFunc_1;
	Il2CppIUnknown* ___getTrailersFunc_2;
	Il2CppIUnknown* ___disposeAction_3;
	Il2CppIUnknown* ___callbackState_4;
};

// System.Runtime.CompilerServices.AsyncMethodBuilderCore
struct AsyncMethodBuilderCore_tD5ABB3A2536319A3345B32A5481E37E23DD8CEDF 
{
	// System.Runtime.CompilerServices.IAsyncStateMachine System.Runtime.CompilerServices.AsyncMethodBuilderCore::m_stateMachine
	RuntimeObject* ___m_stateMachine_0;
	// System.Action System.Runtime.CompilerServices.AsyncMethodBuilderCore::m_defaultContextAction
	Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07* ___m_defaultContextAction_1;
};
// Native definition for P/Invoke marshalling of System.Runtime.CompilerServices.AsyncMethodBuilderCore
struct AsyncMethodBuilderCore_tD5ABB3A2536319A3345B32A5481E37E23DD8CEDF_marshaled_pinvoke
{
	RuntimeObject* ___m_stateMachine_0;
	Il2CppMethodPointer ___m_defaultContextAction_1;
};
// Native definition for COM marshalling of System.Runtime.CompilerServices.AsyncMethodBuilderCore
struct AsyncMethodBuilderCore_tD5ABB3A2536319A3345B32A5481E37E23DD8CEDF_marshaled_com
{
	RuntimeObject* ___m_stateMachine_0;
	Il2CppMethodPointer ___m_defaultContextAction_1;
};

// Grpc.Core.BindServiceMethodAttribute
struct BindServiceMethodAttribute_tA0C4802BE18E8C3D565943379450CB84CDEE2CDA  : public Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA
{
	// System.Type Grpc.Core.BindServiceMethodAttribute::<BindType>k__BackingField
	Type_t* ___U3CBindTypeU3Ek__BackingField_1;
	// System.String Grpc.Core.BindServiceMethodAttribute::<BindMethodName>k__BackingField
	String_t* ___U3CBindMethodNameU3Ek__BackingField_2;
};

// System.Boolean
struct Boolean_t09A6377A54BE2F9E6985A8149F19234FD7DDFE22 
{
	// System.Boolean System.Boolean::m_value
	bool ___m_value_0;
};

// System.Byte
struct Byte_t94D9231AC217BE4D2E004C4CD32DF6D099EA41A3 
{
	// System.Byte System.Byte::m_value
	uint8_t ___m_value_0;
};

// System.Threading.CancellationToken
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED 
{
	// System.Threading.CancellationTokenSource System.Threading.CancellationToken::_source
	CancellationTokenSource_tAAE1E0033BCFC233801F8CB4CED5C852B350CB7B* ____source_0;
};
// Native definition for P/Invoke marshalling of System.Threading.CancellationToken
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_pinvoke
{
	CancellationTokenSource_tAAE1E0033BCFC233801F8CB4CED5C852B350CB7B* ____source_0;
};
// Native definition for COM marshalling of System.Threading.CancellationToken
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_com
{
	CancellationTokenSource_tAAE1E0033BCFC233801F8CB4CED5C852B350CB7B* ____source_0;
};

// System.Char
struct Char_t521A6F19B456D956AF452D926C32709DC03D6B17 
{
	// System.Char System.Char::m_value
	Il2CppChar ___m_value_0;
};

// System.DateTime
struct DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D 
{
	// System.UInt64 System.DateTime::_dateData
	uint64_t ____dateData_46;
};

// System.Diagnostics.CodeAnalysis.DynamicallyAccessedMembersAttribute
struct DynamicallyAccessedMembersAttribute_t2F26B97D513CC0C186D7433DD2935BDED77C481E  : public Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA
{
	// System.Diagnostics.CodeAnalysis.DynamicallyAccessedMemberTypes System.Diagnostics.CodeAnalysis.DynamicallyAccessedMembersAttribute::<MemberTypes>k__BackingField
	int32_t ___U3CMemberTypesU3Ek__BackingField_0;
};

// Microsoft.CodeAnalysis.EmbeddedAttribute
struct EmbeddedAttribute_t1EE07D5A21E2F81B6A651A3843A9BE8E5ADEE16D  : public Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA
{
};

// System.Int32
struct Int32_t680FF22E76F6EFAD4375103CBBFFA0421349384C 
{
	// System.Int32 System.Int32::m_value
	int32_t ___m_value_0;
};

// System.IntPtr
struct IntPtr_t 
{
	// System.Void* System.IntPtr::m_value
	void* ___m_value_0;
};

// Grpc.Core.Interceptors.InterceptingCallInvoker
struct InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4  : public CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF
{
	// Grpc.Core.CallInvoker Grpc.Core.Interceptors.InterceptingCallInvoker::invoker
	CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___invoker_0;
	// Grpc.Core.Interceptors.Interceptor Grpc.Core.Interceptors.InterceptingCallInvoker::interceptor
	Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* ___interceptor_1;
};

// System.Runtime.CompilerServices.IsReadOnlyAttribute
struct IsReadOnlyAttribute_t0A4E747F0A0347653B3EC39C15485282851A10B2  : public Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA
{
};

// System.Runtime.CompilerServices.NullableAttribute
struct NullableAttribute_t7181E30ED50BDD358745EA005FCEC8C87AEF5EBA  : public Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA
{
	// System.Byte[] System.Runtime.CompilerServices.NullableAttribute::NullableFlags
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___NullableFlags_0;
};

// System.Runtime.CompilerServices.NullableContextAttribute
struct NullableContextAttribute_t4A7A18FD00A42573002F14FA54763772B783B64C  : public Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA
{
	// System.Byte System.Runtime.CompilerServices.NullableContextAttribute::Flag
	uint8_t ___Flag_0;
};

// Grpc.Core.SslCredentials
struct SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3  : public ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730
{
	// System.String Grpc.Core.SslCredentials::rootCertificates
	String_t* ___rootCertificates_2;
	// Grpc.Core.KeyCertificatePair Grpc.Core.SslCredentials::keyCertificatePair
	KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* ___keyCertificatePair_3;
	// Grpc.Core.VerifyPeerCallback Grpc.Core.SslCredentials::verifyPeerCallback
	VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* ___verifyPeerCallback_4;
};

// Grpc.Core.Status
struct Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 
{
	// Grpc.Core.StatusCode Grpc.Core.Status::<StatusCode>k__BackingField
	int32_t ___U3CStatusCodeU3Ek__BackingField_2;
	// System.String Grpc.Core.Status::<Detail>k__BackingField
	String_t* ___U3CDetailU3Ek__BackingField_3;
	// System.Exception Grpc.Core.Status::<DebugException>k__BackingField
	Exception_t* ___U3CDebugExceptionU3Ek__BackingField_4;
};
// Native definition for P/Invoke marshalling of Grpc.Core.Status
struct Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_pinvoke
{
	int32_t ___U3CStatusCodeU3Ek__BackingField_2;
	char* ___U3CDetailU3Ek__BackingField_3;
	Exception_t_marshaled_pinvoke* ___U3CDebugExceptionU3Ek__BackingField_4;
};
// Native definition for COM marshalling of Grpc.Core.Status
struct Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_com
{
	int32_t ___U3CStatusCodeU3Ek__BackingField_2;
	Il2CppChar* ___U3CDetailU3Ek__BackingField_3;
	Exception_t_marshaled_com* ___U3CDebugExceptionU3Ek__BackingField_4;
};

// Grpc.Core.Internal.UnimplementedCallInvoker
struct UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435  : public CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF
{
};

// System.Void
struct Void_t4861ACF8F4594C3437BB48B6E56783494B843915 
{
	union
	{
		struct
		{
		};
		uint8_t Void_t4861ACF8F4594C3437BB48B6E56783494B843915__padding[1];
	};
};

// Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials
struct AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862  : public CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466
{
	// Grpc.Core.AsyncAuthInterceptor Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials::interceptor
	AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* ___interceptor_0;
};

// Grpc.Core.CallCredentials/CompositeCallCredentials
struct CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E  : public CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466
{
	// System.Collections.Generic.IReadOnlyList`1<Grpc.Core.CallCredentials> Grpc.Core.CallCredentials/CompositeCallCredentials::credentials
	RuntimeObject* ___credentials_0;
};

// Grpc.Core.Interceptors.CallInvokerExtensions/MetadataInterceptor
struct MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975  : public Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0
{
	// System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata> Grpc.Core.Interceptors.CallInvokerExtensions/MetadataInterceptor::interceptor
	Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* ___interceptor_0;
};

// Grpc.Core.ChannelCredentials/CompositeChannelCredentials
struct CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740  : public ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730
{
	// Grpc.Core.ChannelCredentials Grpc.Core.ChannelCredentials/CompositeChannelCredentials::channelCredentials
	ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ___channelCredentials_2;
	// Grpc.Core.CallCredentials Grpc.Core.ChannelCredentials/CompositeChannelCredentials::callCredentials
	CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___callCredentials_3;
};

// Grpc.Core.ChannelCredentials/InsecureCredentials
struct InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A  : public ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730
{
};

// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInterceptor
struct ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425  : public Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0
{
	// System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo> Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInterceptor::interceptor
	Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* ___interceptor_0;
};

// System.Runtime.CompilerServices.AsyncTaskMethodBuilder`1<System.Threading.Tasks.VoidTaskResult>
struct AsyncTaskMethodBuilder_1_tE88892A6B2F97B5D44B7C3EE2DBEED85743412AC 
{
	// System.Runtime.CompilerServices.AsyncMethodBuilderCore System.Runtime.CompilerServices.AsyncTaskMethodBuilder`1::m_coreState
	AsyncMethodBuilderCore_tD5ABB3A2536319A3345B32A5481E37E23DD8CEDF ___m_coreState_1;
	// System.Threading.Tasks.Task`1<TResult> System.Runtime.CompilerServices.AsyncTaskMethodBuilder`1::m_task
	Task_1_tE41CFF640EB7C045550D9D0D92BE67533B084C17* ___m_task_2;
};

// System.Nullable`1<System.DateTime>
struct Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC 
{
	// System.Boolean System.Nullable`1::hasValue
	bool ___hasValue_0;
	// T System.Nullable`1::value
	DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___value_1;
};

// System.Delegate
struct Delegate_t  : public RuntimeObject
{
	// System.IntPtr System.Delegate::method_ptr
	Il2CppMethodPointer ___method_ptr_0;
	// System.IntPtr System.Delegate::invoke_impl
	intptr_t ___invoke_impl_1;
	// System.Object System.Delegate::m_target
	RuntimeObject* ___m_target_2;
	// System.IntPtr System.Delegate::method
	intptr_t ___method_3;
	// System.IntPtr System.Delegate::delegate_trampoline
	intptr_t ___delegate_trampoline_4;
	// System.IntPtr System.Delegate::extra_arg
	intptr_t ___extra_arg_5;
	// System.IntPtr System.Delegate::method_code
	intptr_t ___method_code_6;
	// System.IntPtr System.Delegate::interp_method
	intptr_t ___interp_method_7;
	// System.IntPtr System.Delegate::interp_invoke_impl
	intptr_t ___interp_invoke_impl_8;
	// System.Reflection.MethodInfo System.Delegate::method_info
	MethodInfo_t* ___method_info_9;
	// System.Reflection.MethodInfo System.Delegate::original_method_info
	MethodInfo_t* ___original_method_info_10;
	// System.DelegateData System.Delegate::data
	DelegateData_t9B286B493293CD2D23A5B2B5EF0E5B1324C2B77E* ___data_11;
	// System.Boolean System.Delegate::method_is_virtual
	bool ___method_is_virtual_12;
};
// Native definition for P/Invoke marshalling of System.Delegate
struct Delegate_t_marshaled_pinvoke
{
	intptr_t ___method_ptr_0;
	intptr_t ___invoke_impl_1;
	Il2CppIUnknown* ___m_target_2;
	intptr_t ___method_3;
	intptr_t ___delegate_trampoline_4;
	intptr_t ___extra_arg_5;
	intptr_t ___method_code_6;
	intptr_t ___interp_method_7;
	intptr_t ___interp_invoke_impl_8;
	MethodInfo_t* ___method_info_9;
	MethodInfo_t* ___original_method_info_10;
	DelegateData_t9B286B493293CD2D23A5B2B5EF0E5B1324C2B77E* ___data_11;
	int32_t ___method_is_virtual_12;
};
// Native definition for COM marshalling of System.Delegate
struct Delegate_t_marshaled_com
{
	intptr_t ___method_ptr_0;
	intptr_t ___invoke_impl_1;
	Il2CppIUnknown* ___m_target_2;
	intptr_t ___method_3;
	intptr_t ___delegate_trampoline_4;
	intptr_t ___extra_arg_5;
	intptr_t ___method_code_6;
	intptr_t ___interp_method_7;
	intptr_t ___interp_invoke_impl_8;
	MethodInfo_t* ___method_info_9;
	MethodInfo_t* ___original_method_info_10;
	DelegateData_t9B286B493293CD2D23A5B2B5EF0E5B1324C2B77E* ___data_11;
	int32_t ___method_is_virtual_12;
};

// System.Exception
struct Exception_t  : public RuntimeObject
{
	// System.String System.Exception::_className
	String_t* ____className_1;
	// System.String System.Exception::_message
	String_t* ____message_2;
	// System.Collections.IDictionary System.Exception::_data
	RuntimeObject* ____data_3;
	// System.Exception System.Exception::_innerException
	Exception_t* ____innerException_4;
	// System.String System.Exception::_helpURL
	String_t* ____helpURL_5;
	// System.Object System.Exception::_stackTrace
	RuntimeObject* ____stackTrace_6;
	// System.String System.Exception::_stackTraceString
	String_t* ____stackTraceString_7;
	// System.String System.Exception::_remoteStackTraceString
	String_t* ____remoteStackTraceString_8;
	// System.Int32 System.Exception::_remoteStackIndex
	int32_t ____remoteStackIndex_9;
	// System.Object System.Exception::_dynamicMethods
	RuntimeObject* ____dynamicMethods_10;
	// System.Int32 System.Exception::_HResult
	int32_t ____HResult_11;
	// System.String System.Exception::_source
	String_t* ____source_12;
	// System.Runtime.Serialization.SafeSerializationManager System.Exception::_safeSerializationManager
	SafeSerializationManager_tCBB85B95DFD1634237140CD892E82D06ECB3F5E6* ____safeSerializationManager_13;
	// System.Diagnostics.StackTrace[] System.Exception::captured_traces
	StackTraceU5BU5D_t32FBCB20930EAF5BAE3F450FF75228E5450DA0DF* ___captured_traces_14;
	// System.IntPtr[] System.Exception::native_trace_ips
	IntPtrU5BU5D_tFD177F8C806A6921AD7150264CCC62FA00CAD832* ___native_trace_ips_15;
	// System.Int32 System.Exception::caught_in_unmanaged
	int32_t ___caught_in_unmanaged_16;
};
// Native definition for P/Invoke marshalling of System.Exception
struct Exception_t_marshaled_pinvoke
{
	char* ____className_1;
	char* ____message_2;
	RuntimeObject* ____data_3;
	Exception_t_marshaled_pinvoke* ____innerException_4;
	char* ____helpURL_5;
	Il2CppIUnknown* ____stackTrace_6;
	char* ____stackTraceString_7;
	char* ____remoteStackTraceString_8;
	int32_t ____remoteStackIndex_9;
	Il2CppIUnknown* ____dynamicMethods_10;
	int32_t ____HResult_11;
	char* ____source_12;
	SafeSerializationManager_tCBB85B95DFD1634237140CD892E82D06ECB3F5E6* ____safeSerializationManager_13;
	StackTraceU5BU5D_t32FBCB20930EAF5BAE3F450FF75228E5450DA0DF* ___captured_traces_14;
	Il2CppSafeArray/*NONE*/* ___native_trace_ips_15;
	int32_t ___caught_in_unmanaged_16;
};
// Native definition for COM marshalling of System.Exception
struct Exception_t_marshaled_com
{
	Il2CppChar* ____className_1;
	Il2CppChar* ____message_2;
	RuntimeObject* ____data_3;
	Exception_t_marshaled_com* ____innerException_4;
	Il2CppChar* ____helpURL_5;
	Il2CppIUnknown* ____stackTrace_6;
	Il2CppChar* ____stackTraceString_7;
	Il2CppChar* ____remoteStackTraceString_8;
	int32_t ____remoteStackIndex_9;
	Il2CppIUnknown* ____dynamicMethods_10;
	int32_t ____HResult_11;
	Il2CppChar* ____source_12;
	SafeSerializationManager_tCBB85B95DFD1634237140CD892E82D06ECB3F5E6* ____safeSerializationManager_13;
	StackTraceU5BU5D_t32FBCB20930EAF5BAE3F450FF75228E5450DA0DF* ___captured_traces_14;
	Il2CppSafeArray/*NONE*/* ___native_trace_ips_15;
	int32_t ___caught_in_unmanaged_16;
};

// System.RuntimeTypeHandle
struct RuntimeTypeHandle_t332A452B8B6179E4469B69525D0FE82A88030F7B 
{
	// System.IntPtr System.RuntimeTypeHandle::value
	intptr_t ___value_0;
};

// System.Runtime.CompilerServices.AsyncTaskMethodBuilder
struct AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06 
{
	// System.Runtime.CompilerServices.AsyncTaskMethodBuilder`1<System.Threading.Tasks.VoidTaskResult> System.Runtime.CompilerServices.AsyncTaskMethodBuilder::m_builder
	AsyncTaskMethodBuilder_1_tE88892A6B2F97B5D44B7C3EE2DBEED85743412AC ___m_builder_1;
};
// Native definition for P/Invoke marshalling of System.Runtime.CompilerServices.AsyncTaskMethodBuilder
struct AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06_marshaled_pinvoke
{
	AsyncTaskMethodBuilder_1_tE88892A6B2F97B5D44B7C3EE2DBEED85743412AC ___m_builder_1;
};
// Native definition for COM marshalling of System.Runtime.CompilerServices.AsyncTaskMethodBuilder
struct AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06_marshaled_com
{
	AsyncTaskMethodBuilder_1_tE88892A6B2F97B5D44B7C3EE2DBEED85743412AC ___m_builder_1;
};

// Grpc.Core.CallOptions
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E 
{
	// Grpc.Core.Metadata Grpc.Core.CallOptions::headers
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___headers_0;
	// System.Nullable`1<System.DateTime> Grpc.Core.CallOptions::deadline
	Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC ___deadline_1;
	// System.Threading.CancellationToken Grpc.Core.CallOptions::cancellationToken
	CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ___cancellationToken_2;
	// Grpc.Core.WriteOptions Grpc.Core.CallOptions::writeOptions
	WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___writeOptions_3;
	// Grpc.Core.ContextPropagationToken Grpc.Core.CallOptions::propagationToken
	ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___propagationToken_4;
	// Grpc.Core.CallCredentials Grpc.Core.CallOptions::credentials
	CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___credentials_5;
	// Grpc.Core.Internal.CallFlags Grpc.Core.CallOptions::flags
	int32_t ___flags_6;
};
// Native definition for P/Invoke marshalling of Grpc.Core.CallOptions
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke
{
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___headers_0;
	Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC ___deadline_1;
	CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_pinvoke ___cancellationToken_2;
	WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___writeOptions_3;
	ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___propagationToken_4;
	CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___credentials_5;
	int32_t ___flags_6;
};
// Native definition for COM marshalling of Grpc.Core.CallOptions
struct CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com
{
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___headers_0;
	Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC ___deadline_1;
	CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_com ___cancellationToken_2;
	WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___writeOptions_3;
	ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___propagationToken_4;
	CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___credentials_5;
	int32_t ___flags_6;
};

// System.MulticastDelegate
struct MulticastDelegate_t  : public Delegate_t
{
	// System.Delegate[] System.MulticastDelegate::delegates
	DelegateU5BU5D_tC5AB7E8F745616680F337909D3A8E6C722CDF771* ___delegates_13;
};
// Native definition for P/Invoke marshalling of System.MulticastDelegate
struct MulticastDelegate_t_marshaled_pinvoke : public Delegate_t_marshaled_pinvoke
{
	Delegate_t_marshaled_pinvoke** ___delegates_13;
};
// Native definition for COM marshalling of System.MulticastDelegate
struct MulticastDelegate_t_marshaled_com : public Delegate_t_marshaled_com
{
	Delegate_t_marshaled_com** ___delegates_13;
};

// Grpc.Core.RpcException
struct RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72  : public Exception_t
{
	// Grpc.Core.Status Grpc.Core.RpcException::status
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___status_18;
	// Grpc.Core.Metadata Grpc.Core.RpcException::trailers
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___trailers_19;
};

// System.SystemException
struct SystemException_tCC48D868298F4C0705279823E34B00F4FBDB7295  : public Exception_t
{
};

// System.Type
struct Type_t  : public MemberInfo_t
{
	// System.RuntimeTypeHandle System.Type::_impl
	RuntimeTypeHandle_t332A452B8B6179E4469B69525D0FE82A88030F7B ____impl_8;
};

// System.Action`1<System.Object>
struct Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87  : public MulticastDelegate_t
{
};

// System.Action`1<Grpc.Core.ServiceBinderBase>
struct Action_1_t805B4C224C6B55AF87988396EE027CA269CD12EE  : public MulticastDelegate_t
{
};

// System.Func`1<Grpc.Core.Metadata>
struct Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C  : public MulticastDelegate_t
{
};

// System.Func`1<System.Object>
struct Func_1_tD5C081AE11746B200C711DD48DBEB00E3A9276D4  : public MulticastDelegate_t
{
};

// System.Func`1<Grpc.Core.Status>
struct Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0  : public MulticastDelegate_t
{
};

// System.Func`2<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>,System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty>>
struct Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A  : public MulticastDelegate_t
{
};

// System.Func`2<System.Byte[],System.String>
struct Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF  : public MulticastDelegate_t
{
};

// System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>
struct Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B  : public MulticastDelegate_t
{
};

// System.Func`2<System.Object,System.Threading.Tasks.Task`1<Grpc.Core.Metadata>>
struct Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974  : public MulticastDelegate_t
{
};

// System.Func`2<System.Object,Grpc.Core.Metadata>
struct Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A  : public MulticastDelegate_t
{
};

// System.Func`2<System.Object,System.Object>
struct Func_2_tACBF5A1656250800CE861707354491F0611F6624  : public MulticastDelegate_t
{
};

// System.Func`2<System.Object,Grpc.Core.Status>
struct Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2  : public MulticastDelegate_t
{
};

// System.Func`2<System.String,System.Byte[]>
struct Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C  : public MulticastDelegate_t
{
};

// System.Action
struct Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07  : public MulticastDelegate_t
{
};

// System.ArgumentException
struct ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263  : public SystemException_tCC48D868298F4C0705279823E34B00F4FBDB7295
{
	// System.String System.ArgumentException::_paramName
	String_t* ____paramName_18;
};

// Grpc.Core.AsyncAuthInterceptor
struct AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968  : public MulticastDelegate_t
{
};

// System.AsyncCallback
struct AsyncCallback_t7FEF460CBDCFB9C5FA2EF776984778B9A4145F4C  : public MulticastDelegate_t
{
};

// System.InvalidOperationException
struct InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB  : public SystemException_tCC48D868298F4C0705279823E34B00F4FBDB7295
{
};

// System.NotImplementedException
struct NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8  : public SystemException_tCC48D868298F4C0705279823E34B00F4FBDB7295
{
};

// System.NotSupportedException
struct NotSupportedException_t1429765983D409BD2986508963C98D214E4EBF4A  : public SystemException_tCC48D868298F4C0705279823E34B00F4FBDB7295
{
};

// Grpc.Core.VerifyPeerCallback
struct VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71  : public MulticastDelegate_t
{
};

// Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6
struct U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44 
{
	// System.Int32 Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6::<>1__state
	int32_t ___U3CU3E1__state_0;
	// System.Runtime.CompilerServices.AsyncTaskMethodBuilder Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6::<>t__builder
	AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06 ___U3CU3Et__builder_1;
};

// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo
struct ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9 
{
	// System.String Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo::Host
	String_t* ___Host_0;
	// Grpc.Core.CallOptions Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo::CallOptions
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E ___CallOptions_1;
};
// Native definition for P/Invoke marshalling of Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo
struct ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_pinvoke
{
	char* ___Host_0;
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke ___CallOptions_1;
};
// Native definition for COM marshalling of Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo
struct ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_com
{
	Il2CppChar* ___Host_0;
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com ___CallOptions_1;
};

// System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>
struct Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20  : public MulticastDelegate_t
{
};

// <Module>

// <Module>

// System.Collections.Generic.Dictionary`2<System.Object,System.Object>

// System.Collections.Generic.Dictionary`2<System.Object,System.Object>

// System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>

// System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>

// System.Collections.Generic.Dictionary`2<System.String,System.Object>

// System.Collections.Generic.Dictionary`2<System.String,System.Object>

// System.Linq.EmptyEnumerable`1<System.Object>
struct EmptyEnumerable_1_t8C8873EF4F89FB0F86D91BA5B4D640E3A23AD28E_StaticFields
{
	// TElement[] System.Linq.EmptyEnumerable`1::Instance
	ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* ___Instance_0;
};

// System.Linq.EmptyEnumerable`1<System.Object>

// System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>
struct List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5_StaticFields
{
	// T[] System.Collections.Generic.List`1::s_emptyArray
	Action_1U5BU5D_tEEB96F533EF5D717115A455D9AB8389102E41757* ___s_emptyArray_5;
};

// System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>

// System.Collections.Generic.List`1<Grpc.Core.AuthProperty>
struct List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A_StaticFields
{
	// T[] System.Collections.Generic.List`1::s_emptyArray
	AuthPropertyU5BU5D_t1B9314F35919A8B7A7BEA7BC89C18B4D3AA69A76* ___s_emptyArray_5;
};

// System.Collections.Generic.List`1<Grpc.Core.AuthProperty>

// System.Collections.Generic.List`1<Grpc.Core.CallCredentials>
struct List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1_StaticFields
{
	// T[] System.Collections.Generic.List`1::s_emptyArray
	CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* ___s_emptyArray_5;
};

// System.Collections.Generic.List`1<Grpc.Core.CallCredentials>

// System.Collections.Generic.List`1<System.Object>
struct List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D_StaticFields
{
	// T[] System.Collections.Generic.List`1::s_emptyArray
	ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* ___s_emptyArray_5;
};

// System.Collections.Generic.List`1<System.Object>

// System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>
struct List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337_StaticFields
{
	// T[] System.Collections.Generic.List`1::s_emptyArray
	EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4* ___s_emptyArray_5;
};

// System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>

// Grpc.Core.Marshaller`1<System.String>

// Grpc.Core.Marshaller`1<System.String>

// System.Collections.ObjectModel.ReadOnlyCollection`1<System.Action`1<Grpc.Core.ServiceBinderBase>>

// System.Collections.ObjectModel.ReadOnlyCollection`1<System.Action`1<Grpc.Core.ServiceBinderBase>>

// System.Collections.ObjectModel.ReadOnlyCollection`1<Grpc.Core.CallCredentials>

// System.Collections.ObjectModel.ReadOnlyCollection`1<Grpc.Core.CallCredentials>

// System.Collections.Generic.Dictionary`2/ValueCollection<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>

// System.Collections.Generic.Dictionary`2/ValueCollection<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>

// Grpc.Core.AsyncStreamReaderExtensions

// Grpc.Core.AsyncStreamReaderExtensions

// System.Attribute

// System.Attribute

// Grpc.Core.AuthContext

// Grpc.Core.AuthContext

// Grpc.Core.AuthInterceptorContext

// Grpc.Core.AuthInterceptorContext

// Grpc.Core.AuthProperty
struct AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_StaticFields
{
	// System.Text.Encoding Grpc.Core.AuthProperty::EncodingUTF8
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___EncodingUTF8_0;
};

// Grpc.Core.AuthProperty

// Grpc.Core.CallCredentials

// Grpc.Core.CallCredentials

// Grpc.Core.CallCredentialsConfiguratorBase

// Grpc.Core.CallCredentialsConfiguratorBase

// Grpc.Core.CallInvoker

// Grpc.Core.CallInvoker

// Grpc.Core.Interceptors.CallInvokerExtensions

// Grpc.Core.Interceptors.CallInvokerExtensions

// Grpc.Core.ChannelBase

// Grpc.Core.ChannelBase

// Grpc.Core.ChannelCredentials
struct ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_StaticFields
{
	// Grpc.Core.ChannelCredentials Grpc.Core.ChannelCredentials::InsecureInstance
	ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ___InsecureInstance_0;
	// Grpc.Core.ChannelCredentials Grpc.Core.ChannelCredentials::SecureSslInstance
	ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ___SecureSslInstance_1;
};

// Grpc.Core.ChannelCredentials

// Grpc.Core.ChannelCredentialsConfiguratorBase

// Grpc.Core.ChannelCredentialsConfiguratorBase

// Grpc.Core.Interceptors.ChannelExtensions

// Grpc.Core.Interceptors.ChannelExtensions

// Grpc.Core.ClientBase

// Grpc.Core.ClientBase

// Grpc.Core.ContextPropagationOptions
struct ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_StaticFields
{
	// Grpc.Core.ContextPropagationOptions Grpc.Core.ContextPropagationOptions::Default
	ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* ___Default_0;
};

// Grpc.Core.ContextPropagationOptions

// Grpc.Core.ContextPropagationToken

// Grpc.Core.ContextPropagationToken

// Grpc.Core.DeserializationContext

// Grpc.Core.DeserializationContext

// System.Text.Encoding
struct Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095_StaticFields
{
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::defaultEncoding
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___defaultEncoding_0;
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::unicodeEncoding
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___unicodeEncoding_1;
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::bigEndianUnicode
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___bigEndianUnicode_2;
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::utf7Encoding
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___utf7Encoding_3;
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::utf8Encoding
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___utf8Encoding_4;
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::utf32Encoding
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___utf32Encoding_5;
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::asciiEncoding
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___asciiEncoding_6;
	// System.Text.Encoding modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::latin1Encoding
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___latin1Encoding_7;
	// System.Collections.Generic.Dictionary`2<System.Int32,System.Text.Encoding> modreq(System.Runtime.CompilerServices.IsVolatile) System.Text.Encoding::encodings
	Dictionary_2_t87EDE08B2E48F793A22DE50D6B3CC2E7EBB2DB54* ___encodings_8;
	// System.Object System.Text.Encoding::s_InternalSyncObject
	RuntimeObject* ___s_InternalSyncObject_15;
};

// System.Text.Encoding

// Grpc.Core.Api.Utils.EncodingExtensions

// Grpc.Core.Api.Utils.EncodingExtensions

// Grpc.Core.Utils.GrpcPreconditions

// Grpc.Core.Utils.GrpcPreconditions

// Grpc.Core.Interceptors.Interceptor

// Grpc.Core.Interceptors.Interceptor

// Grpc.Core.KeyCertificatePair

// Grpc.Core.KeyCertificatePair

// Grpc.Core.Marshallers

// Grpc.Core.Marshallers

// System.Reflection.MemberInfo

// System.Reflection.MemberInfo

// Grpc.Core.Metadata
struct Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields
{
	// Grpc.Core.Metadata Grpc.Core.Metadata::Empty
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___Empty_1;
	// System.Text.Encoding Grpc.Core.Metadata::EncodingASCII
	Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___EncodingASCII_3;
};

// Grpc.Core.Metadata

// Grpc.Core.SerializationContext

// Grpc.Core.SerializationContext

// Grpc.Core.ServerCallContext

// Grpc.Core.ServerCallContext

// Grpc.Core.ServerServiceDefinition

// Grpc.Core.ServerServiceDefinition

// Grpc.Core.ServiceBinderBase

// Grpc.Core.ServiceBinderBase

// System.String
struct String_t_StaticFields
{
	// System.String System.String::Empty
	String_t* ___Empty_6;
};

// System.String

// System.Threading.Tasks.Task
struct Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572_StaticFields
{
	// System.Int32 System.Threading.Tasks.Task::s_taskIdCounter
	int32_t ___s_taskIdCounter_0;
	// System.Object System.Threading.Tasks.Task::s_taskCompletionSentinel
	RuntimeObject* ___s_taskCompletionSentinel_24;
	// System.Boolean System.Threading.Tasks.Task::s_asyncDebuggingEnabled
	bool ___s_asyncDebuggingEnabled_25;
	// System.Action`1<System.Object> System.Threading.Tasks.Task::s_taskCancelCallback
	Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* ___s_taskCancelCallback_27;
	// System.Func`1<System.Threading.Tasks.Task/ContingentProperties> System.Threading.Tasks.Task::s_createContingentProperties
	Func_1_tD59A12717D79BFB403BF973694B1BE5B85474BD1* ___s_createContingentProperties_30;
	// System.Threading.Tasks.TaskFactory System.Threading.Tasks.Task::<Factory>k__BackingField
	TaskFactory_tF781BD37BE23917412AD83424D1497C7C1509DF0* ___U3CFactoryU3Ek__BackingField_31;
	// System.Threading.Tasks.Task System.Threading.Tasks.Task::<CompletedTask>k__BackingField
	Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* ___U3CCompletedTaskU3Ek__BackingField_32;
	// System.Predicate`1<System.Threading.Tasks.Task> System.Threading.Tasks.Task::s_IsExceptionObservedByParentPredicate
	Predicate_1_t7F48518B008C1472339EEEBABA3DE203FE1F26ED* ___s_IsExceptionObservedByParentPredicate_33;
	// System.Threading.ContextCallback System.Threading.Tasks.Task::s_ecCallback
	ContextCallback_tE8AFBDBFCC040FDA8DA8C1EEFE9BD66B16BDA007* ___s_ecCallback_34;
	// System.Predicate`1<System.Object> System.Threading.Tasks.Task::s_IsTaskContinuationNullPredicate
	Predicate_1_t8342C85FF4E41CD1F7024AC0CDC3E5312A32CB12* ___s_IsTaskContinuationNullPredicate_35;
	// System.Collections.Generic.Dictionary`2<System.Int32,System.Threading.Tasks.Task> System.Threading.Tasks.Task::s_currentActiveTasks
	Dictionary_2_t403063CE4960B4F46C688912237C6A27E550FF55* ___s_currentActiveTasks_36;
	// System.Object System.Threading.Tasks.Task::s_activeTasksLock
	RuntimeObject* ___s_activeTasksLock_37;
};

// System.Threading.Tasks.Task
struct Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572_ThreadStaticFields
{
	// System.Threading.Tasks.Task System.Threading.Tasks.Task::t_currentTask
	Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* ___t_currentTask_28;
	// System.Threading.Tasks.StackGuard System.Threading.Tasks.Task::t_stackGuard
	StackGuard_tACE063A1B7374BDF4AD472DE4585D05AD8745352* ___t_stackGuard_29;
};

// Grpc.Core.VerifyPeerContext

// Grpc.Core.VerifyPeerContext

// Grpc.Core.VersionInfo

// Grpc.Core.VersionInfo

// Grpc.Core.WriteOptions
struct WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_StaticFields
{
	// Grpc.Core.WriteOptions Grpc.Core.WriteOptions::Default
	WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___Default_0;
};

// Grpc.Core.WriteOptions

// Grpc.Core.AuthContext/<>c
struct U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_StaticFields
{
	// Grpc.Core.AuthContext/<>c Grpc.Core.AuthContext/<>c::<>9
	U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677* ___U3CU3E9_0;
	// System.Func`2<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>,System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty>> Grpc.Core.AuthContext/<>c::<>9__10_0
	Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* ___U3CU3E9__10_0_1;
};

// Grpc.Core.AuthContext/<>c

// Grpc.Core.ClientBase/ClientBaseConfiguration

// Grpc.Core.ClientBase/ClientBaseConfiguration

// Grpc.Core.Metadata/<GetAll>d__11

// Grpc.Core.Metadata/<GetAll>d__11

// Grpc.Core.Metadata/Entry
struct Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_StaticFields
{
	// System.Byte[] Grpc.Core.Metadata/Entry::EmptyByteArray
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___EmptyByteArray_3;
};

// Grpc.Core.Metadata/Entry

// Grpc.Core.ServerServiceDefinition/Builder

// Grpc.Core.ServerServiceDefinition/Builder

// System.Collections.Generic.List`1/Enumerator<System.Object>

// System.Collections.Generic.List`1/Enumerator<System.Object>

// System.Collections.Generic.List`1/Enumerator<Grpc.Core.Metadata/Entry>

// System.Collections.Generic.List`1/Enumerator<Grpc.Core.Metadata/Entry>

// System.Buffers.ReadOnlySequence`1<System.Byte>
struct ReadOnlySequence_1_t85097489C00952FC58DF9F3ACE926EAE9A51898A_StaticFields
{
	// System.Buffers.ReadOnlySequence`1<T> System.Buffers.ReadOnlySequence`1::Empty
	ReadOnlySequence_1_t85097489C00952FC58DF9F3ACE926EAE9A51898A ___Empty_4;
};

// System.Buffers.ReadOnlySequence`1<System.Byte>

// System.Threading.Tasks.Task`1<Grpc.Core.Metadata>
struct Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707_StaticFields
{
	// System.Threading.Tasks.TaskFactory`1<TResult> System.Threading.Tasks.Task`1::s_defaultFactory
	TaskFactory_1_tDAEC71AE5F736E76304BA6958D048E7AE8FF6AA9* ___s_defaultFactory_39;
};

// System.Threading.Tasks.Task`1<Grpc.Core.Metadata>

// Grpc.Core.AsyncCallState

// Grpc.Core.AsyncCallState

// Grpc.Core.BindServiceMethodAttribute

// Grpc.Core.BindServiceMethodAttribute

// System.Boolean
struct Boolean_t09A6377A54BE2F9E6985A8149F19234FD7DDFE22_StaticFields
{
	// System.String System.Boolean::TrueString
	String_t* ___TrueString_5;
	// System.String System.Boolean::FalseString
	String_t* ___FalseString_6;
};

// System.Boolean

// System.Byte

// System.Byte

// System.Threading.CancellationToken
struct CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_StaticFields
{
	// System.Action`1<System.Object> System.Threading.CancellationToken::s_actionToActionObjShunt
	Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* ___s_actionToActionObjShunt_1;
};

// System.Threading.CancellationToken

// System.Char
struct Char_t521A6F19B456D956AF452D926C32709DC03D6B17_StaticFields
{
	// System.Byte[] System.Char::s_categoryForLatin1
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___s_categoryForLatin1_3;
};

// System.Char

// System.DateTime
struct DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D_StaticFields
{
	// System.Int32[] System.DateTime::s_daysToMonth365
	Int32U5BU5D_t19C97395396A72ECAF310612F0760F165060314C* ___s_daysToMonth365_30;
	// System.Int32[] System.DateTime::s_daysToMonth366
	Int32U5BU5D_t19C97395396A72ECAF310612F0760F165060314C* ___s_daysToMonth366_31;
	// System.DateTime System.DateTime::MinValue
	DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___MinValue_32;
	// System.DateTime System.DateTime::MaxValue
	DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___MaxValue_33;
	// System.DateTime System.DateTime::UnixEpoch
	DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___UnixEpoch_34;
};

// System.DateTime

// System.Diagnostics.CodeAnalysis.DynamicallyAccessedMembersAttribute

// System.Diagnostics.CodeAnalysis.DynamicallyAccessedMembersAttribute

// Microsoft.CodeAnalysis.EmbeddedAttribute

// Microsoft.CodeAnalysis.EmbeddedAttribute

// System.Int32

// System.Int32

// System.IntPtr
struct IntPtr_t_StaticFields
{
	// System.IntPtr System.IntPtr::Zero
	intptr_t ___Zero_1;
};

// System.IntPtr

// Grpc.Core.Interceptors.InterceptingCallInvoker

// Grpc.Core.Interceptors.InterceptingCallInvoker

// System.Runtime.CompilerServices.IsReadOnlyAttribute

// System.Runtime.CompilerServices.IsReadOnlyAttribute

// System.Runtime.CompilerServices.NullableAttribute

// System.Runtime.CompilerServices.NullableAttribute

// System.Runtime.CompilerServices.NullableContextAttribute

// System.Runtime.CompilerServices.NullableContextAttribute

// Grpc.Core.SslCredentials

// Grpc.Core.SslCredentials

// Grpc.Core.Status
struct Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_StaticFields
{
	// Grpc.Core.Status Grpc.Core.Status::DefaultSuccess
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___DefaultSuccess_0;
	// Grpc.Core.Status Grpc.Core.Status::DefaultCancelled
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___DefaultCancelled_1;
};

// Grpc.Core.Status

// Grpc.Core.Internal.UnimplementedCallInvoker

// Grpc.Core.Internal.UnimplementedCallInvoker

// System.Void

// System.Void

// Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials

// Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials

// Grpc.Core.CallCredentials/CompositeCallCredentials

// Grpc.Core.CallCredentials/CompositeCallCredentials

// Grpc.Core.Interceptors.CallInvokerExtensions/MetadataInterceptor

// Grpc.Core.Interceptors.CallInvokerExtensions/MetadataInterceptor

// Grpc.Core.ChannelCredentials/CompositeChannelCredentials

// Grpc.Core.ChannelCredentials/CompositeChannelCredentials

// Grpc.Core.ChannelCredentials/InsecureCredentials

// Grpc.Core.ChannelCredentials/InsecureCredentials

// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInterceptor

// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInterceptor

// System.Nullable`1<System.DateTime>

// System.Nullable`1<System.DateTime>

// System.Delegate

// System.Delegate

// System.Exception
struct Exception_t_StaticFields
{
	// System.Object System.Exception::s_EDILock
	RuntimeObject* ___s_EDILock_0;
};

// System.Exception

// System.Runtime.CompilerServices.AsyncTaskMethodBuilder
struct AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06_StaticFields
{
	// System.Threading.Tasks.Task`1<System.Threading.Tasks.VoidTaskResult> System.Runtime.CompilerServices.AsyncTaskMethodBuilder::s_cachedCompleted
	Task_1_tE41CFF640EB7C045550D9D0D92BE67533B084C17* ___s_cachedCompleted_0;
};

// System.Runtime.CompilerServices.AsyncTaskMethodBuilder

// Grpc.Core.CallOptions

// Grpc.Core.CallOptions

// Grpc.Core.RpcException

// Grpc.Core.RpcException

// System.Type
struct Type_t_StaticFields
{
	// System.Reflection.Binder modreq(System.Runtime.CompilerServices.IsVolatile) System.Type::s_defaultBinder
	Binder_t91BFCE95A7057FADF4D8A1A342AFE52872246235* ___s_defaultBinder_0;
	// System.Char System.Type::Delimiter
	Il2CppChar ___Delimiter_1;
	// System.Type[] System.Type::EmptyTypes
	TypeU5BU5D_t97234E1129B564EB38B8D85CAC2AD8B5B9522FFB* ___EmptyTypes_2;
	// System.Object System.Type::Missing
	RuntimeObject* ___Missing_3;
	// System.Reflection.MemberFilter System.Type::FilterAttribute
	MemberFilter_tF644F1AE82F611B677CE1964D5A3277DDA21D553* ___FilterAttribute_4;
	// System.Reflection.MemberFilter System.Type::FilterName
	MemberFilter_tF644F1AE82F611B677CE1964D5A3277DDA21D553* ___FilterName_5;
	// System.Reflection.MemberFilter System.Type::FilterNameIgnoreCase
	MemberFilter_tF644F1AE82F611B677CE1964D5A3277DDA21D553* ___FilterNameIgnoreCase_6;
};

// System.Type

// System.Action`1<System.Object>

// System.Action`1<System.Object>

// System.Action`1<Grpc.Core.ServiceBinderBase>

// System.Action`1<Grpc.Core.ServiceBinderBase>

// System.Func`1<Grpc.Core.Metadata>

// System.Func`1<Grpc.Core.Metadata>

// System.Func`1<System.Object>

// System.Func`1<System.Object>

// System.Func`1<Grpc.Core.Status>

// System.Func`1<Grpc.Core.Status>

// System.Func`2<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>,System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty>>

// System.Func`2<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>,System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty>>

// System.Func`2<System.Byte[],System.String>

// System.Func`2<System.Byte[],System.String>

// System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>

// System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>

// System.Func`2<System.Object,System.Threading.Tasks.Task`1<Grpc.Core.Metadata>>

// System.Func`2<System.Object,System.Threading.Tasks.Task`1<Grpc.Core.Metadata>>

// System.Func`2<System.Object,Grpc.Core.Metadata>

// System.Func`2<System.Object,Grpc.Core.Metadata>

// System.Func`2<System.Object,System.Object>

// System.Func`2<System.Object,System.Object>

// System.Func`2<System.Object,Grpc.Core.Status>

// System.Func`2<System.Object,Grpc.Core.Status>

// System.Func`2<System.String,System.Byte[]>

// System.Func`2<System.String,System.Byte[]>

// System.Action

// System.Action

// System.ArgumentException

// System.ArgumentException

// Grpc.Core.AsyncAuthInterceptor

// Grpc.Core.AsyncAuthInterceptor

// System.AsyncCallback

// System.AsyncCallback

// System.InvalidOperationException

// System.InvalidOperationException

// System.NotImplementedException

// System.NotImplementedException

// System.NotSupportedException

// System.NotSupportedException

// Grpc.Core.VerifyPeerCallback

// Grpc.Core.VerifyPeerCallback

// Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6

// Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6

// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo

// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo

// System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>

// System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
// System.Byte[]
struct ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031  : public RuntimeArray
{
	ALIGN_FIELD (8) uint8_t m_Items[1];

	inline uint8_t GetAt(il2cpp_array_size_t index) const
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items[index];
	}
	inline uint8_t* GetAddressAt(il2cpp_array_size_t index)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items + index;
	}
	inline void SetAt(il2cpp_array_size_t index, uint8_t value)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		m_Items[index] = value;
	}
	inline uint8_t GetAtUnchecked(il2cpp_array_size_t index) const
	{
		return m_Items[index];
	}
	inline uint8_t* GetAddressAtUnchecked(il2cpp_array_size_t index)
	{
		return m_Items + index;
	}
	inline void SetAtUnchecked(il2cpp_array_size_t index, uint8_t value)
	{
		m_Items[index] = value;
	}
};
// System.Delegate[]
struct DelegateU5BU5D_tC5AB7E8F745616680F337909D3A8E6C722CDF771  : public RuntimeArray
{
	ALIGN_FIELD (8) Delegate_t* m_Items[1];

	inline Delegate_t* GetAt(il2cpp_array_size_t index) const
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items[index];
	}
	inline Delegate_t** GetAddressAt(il2cpp_array_size_t index)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items + index;
	}
	inline void SetAt(il2cpp_array_size_t index, Delegate_t* value)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
	inline Delegate_t* GetAtUnchecked(il2cpp_array_size_t index) const
	{
		return m_Items[index];
	}
	inline Delegate_t** GetAddressAtUnchecked(il2cpp_array_size_t index)
	{
		return m_Items + index;
	}
	inline void SetAtUnchecked(il2cpp_array_size_t index, Delegate_t* value)
	{
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
};
// Grpc.Core.CallCredentials[]
struct CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD  : public RuntimeArray
{
	ALIGN_FIELD (8) CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* m_Items[1];

	inline CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* GetAt(il2cpp_array_size_t index) const
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items[index];
	}
	inline CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466** GetAddressAt(il2cpp_array_size_t index)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items + index;
	}
	inline void SetAt(il2cpp_array_size_t index, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* value)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
	inline CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* GetAtUnchecked(il2cpp_array_size_t index) const
	{
		return m_Items[index];
	}
	inline CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466** GetAddressAtUnchecked(il2cpp_array_size_t index)
	{
		return m_Items + index;
	}
	inline void SetAtUnchecked(il2cpp_array_size_t index, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* value)
	{
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
};
// Grpc.Core.Metadata/Entry[]
struct EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4  : public RuntimeArray
{
	ALIGN_FIELD (8) Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* m_Items[1];

	inline Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* GetAt(il2cpp_array_size_t index) const
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items[index];
	}
	inline Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB** GetAddressAt(il2cpp_array_size_t index)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items + index;
	}
	inline void SetAt(il2cpp_array_size_t index, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* value)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
	inline Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* GetAtUnchecked(il2cpp_array_size_t index) const
	{
		return m_Items[index];
	}
	inline Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB** GetAddressAtUnchecked(il2cpp_array_size_t index)
	{
		return m_Items + index;
	}
	inline void SetAtUnchecked(il2cpp_array_size_t index, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* value)
	{
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
};
// Grpc.Core.Interceptors.Interceptor[]
struct InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2  : public RuntimeArray
{
	ALIGN_FIELD (8) Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* m_Items[1];

	inline Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* GetAt(il2cpp_array_size_t index) const
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items[index];
	}
	inline Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0** GetAddressAt(il2cpp_array_size_t index)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items + index;
	}
	inline void SetAt(il2cpp_array_size_t index, Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* value)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
	inline Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* GetAtUnchecked(il2cpp_array_size_t index) const
	{
		return m_Items[index];
	}
	inline Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0** GetAddressAtUnchecked(il2cpp_array_size_t index)
	{
		return m_Items + index;
	}
	inline void SetAtUnchecked(il2cpp_array_size_t index, Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* value)
	{
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
};
// System.Object[]
struct ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918  : public RuntimeArray
{
	ALIGN_FIELD (8) RuntimeObject* m_Items[1];

	inline RuntimeObject* GetAt(il2cpp_array_size_t index) const
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items[index];
	}
	inline RuntimeObject** GetAddressAt(il2cpp_array_size_t index)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		return m_Items + index;
	}
	inline void SetAt(il2cpp_array_size_t index, RuntimeObject* value)
	{
		IL2CPP_ARRAY_BOUNDS_CHECK(index, (uint32_t)(this)->max_length);
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
	inline RuntimeObject* GetAtUnchecked(il2cpp_array_size_t index) const
	{
		return m_Items[index];
	}
	inline RuntimeObject** GetAddressAtUnchecked(il2cpp_array_size_t index)
	{
		return m_Items + index;
	}
	inline void SetAtUnchecked(il2cpp_array_size_t index, RuntimeObject* value)
	{
		m_Items[index] = value;
		Il2CppCodeGenWriteBarrier((void**)m_Items + index, (void*)value);
	}
};

IL2CPP_EXTERN_C void CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshal_pinvoke(const CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED& unmarshaled, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_pinvoke& marshaled);
IL2CPP_EXTERN_C void CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshal_pinvoke_back(const CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_pinvoke& marshaled, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED& unmarshaled);
IL2CPP_EXTERN_C void CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshal_pinvoke_cleanup(CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_pinvoke& marshaled);
IL2CPP_EXTERN_C void CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshal_com(const CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED& unmarshaled, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_com& marshaled);
IL2CPP_EXTERN_C void CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshal_com_back(const CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_com& marshaled, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED& unmarshaled);
IL2CPP_EXTERN_C void CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshal_com_cleanup(CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED_marshaled_com& marshaled);
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_pinvoke(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke& marshaled);
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_pinvoke_back(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke& marshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled);
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_pinvoke_cleanup(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke& marshaled);
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_com(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com& marshaled);
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_com_back(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com& marshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled);
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_com_cleanup(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com& marshaled);
IL2CPP_EXTERN_C void Exception_t_marshal_pinvoke(const Exception_t& unmarshaled, Exception_t_marshaled_pinvoke& marshaled);
IL2CPP_EXTERN_C void Exception_t_marshal_pinvoke_back(const Exception_t_marshaled_pinvoke& marshaled, Exception_t& unmarshaled);
IL2CPP_EXTERN_C void Exception_t_marshal_pinvoke_cleanup(Exception_t_marshaled_pinvoke& marshaled);
IL2CPP_EXTERN_C void Exception_t_marshal_com(const Exception_t& unmarshaled, Exception_t_marshaled_com& marshaled);
IL2CPP_EXTERN_C void Exception_t_marshal_com_back(const Exception_t_marshaled_com& marshaled, Exception_t& unmarshaled);
IL2CPP_EXTERN_C void Exception_t_marshal_com_cleanup(Exception_t_marshaled_com& marshaled);

// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.Object>(T)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared (RuntimeObject* ___0_reference, const RuntimeMethod* method) ;
// TResult System.Func`2<System.Object,System.Object>::Invoke(T)
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR RuntimeObject* Func_2_Invoke_mDBA25DA5DA5B7E056FB9B026AF041F1385FB58A9_gshared_inline (Func_2_tACBF5A1656250800CE861707354491F0611F6624* __this, RuntimeObject* ___0_arg, const RuntimeMethod* method) ;
// TResult System.Func`1<Grpc.Core.Status>::Invoke()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 Func_1_Invoke_m349A2DF2DF3672A6D87AA4E5E86E8AF54B24A956_gshared_inline (Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0* __this, const RuntimeMethod* method) ;
// TResult System.Func`2<System.Object,Grpc.Core.Status>::Invoke(T)
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 Func_2_Invoke_m497B8CC89F1330EB7265E70F432347348B580F14_gshared_inline (Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* __this, RuntimeObject* ___0_arg, const RuntimeMethod* method) ;
// TResult System.Func`1<System.Object>::Invoke()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR RuntimeObject* Func_1_Invoke_m1412272198DFA4066C83206E5B43353AF10A2EEE_gshared_inline (Func_1_tD5C081AE11746B200C711DD48DBEB00E3A9276D4* __this, const RuntimeMethod* method) ;
// System.Void System.Action`1<System.Object>::Invoke(T)
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void Action_1_Invoke_mF2422B2DD29F74CE66F791C3F68E288EC7C3DB9E_gshared_inline (Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* __this, RuntimeObject* ___0_obj, const RuntimeMethod* method) ;
// System.Collections.Generic.IEnumerable`1<TResult> System.Linq.Enumerable::Empty<System.Object>()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR RuntimeObject* Enumerable_Empty_TisRuntimeObject_mA90CDE158774C34A28C07CEEA9E9EA2A61618238_gshared_inline (const RuntimeMethod* method) ;
// TValue System.Collections.Generic.Dictionary`2<System.Object,System.Object>::get_Item(TKey)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* Dictionary_2_get_Item_m4AAAECBE902A211BF2126E6AFA280AEF73A3E0D6_gshared (Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* __this, RuntimeObject* ___0_key, const RuntimeMethod* method) ;
// System.Collections.Generic.Dictionary`2/ValueCollection<TKey,TValue> System.Collections.Generic.Dictionary`2<System.Object,System.Object>::get_Values()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ValueCollection_t038245E04B5D2A80048D9F8021A23E69A0C9DBAA* Dictionary_2_get_Values_mA0C01DEA55329E55380E96BBD04D4D228B437EC5_gshared (Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* __this, const RuntimeMethod* method) ;
// System.Void System.Func`2<System.Object,System.Object>::.ctor(System.Object,System.IntPtr)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Func_2__ctor_m7F8A01C0B02BC1D4063F4EB1E817F7A48562A398_gshared (Func_2_tACBF5A1656250800CE861707354491F0611F6624* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method) ;
// System.Collections.Generic.IEnumerable`1<TResult> System.Linq.Enumerable::SelectMany<System.Object,System.Object>(System.Collections.Generic.IEnumerable`1<TSource>,System.Func`2<TSource,System.Collections.Generic.IEnumerable`1<TResult>>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* Enumerable_SelectMany_TisRuntimeObject_TisRuntimeObject_mC89216034DEE8779F1AC2D0A984C0ADE855BED00_gshared (RuntimeObject* ___0_source, Func_2_t9F45EF9F857977243C345F24571962D2521DB4A1* ___1_selector, const RuntimeMethod* method) ;
// System.Boolean System.Collections.Generic.Dictionary`2<System.Object,System.Object>::TryGetValue(TKey,TValue&)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Dictionary_2_TryGetValue_mD15380A4ED7CDEE99EA45881577D26BA9CE1B849_gshared (Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* __this, RuntimeObject* ___0_key, RuntimeObject** ___1_value, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::.ctor(System.Collections.Generic.IEnumerable`1<T>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void List_1__ctor_mE097DBD72433D1AFC11733F5678602603A756424_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, RuntimeObject* ___0_collection, const RuntimeMethod* method) ;
// System.Collections.ObjectModel.ReadOnlyCollection`1<T> System.Collections.Generic.List`1<System.Object>::AsReadOnly()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ReadOnlyCollection_1_t5397DF0DB61D1090E7BBC89395CECB8D020CED92* List_1_AsReadOnly_m69D18D2353099DC61F87AD42FF87F25B1A4D299B_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, const RuntimeMethod* method) ;
// System.Void System.Nullable`1<System.DateTime>::.ctor(T)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Nullable_1__ctor_mB17304720EA19F5469A4883827F53A75FEB492CF_gshared (Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC* __this, DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___0_value, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.Object>(T,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared (RuntimeObject* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method) ;
// System.Void System.Runtime.CompilerServices.AsyncTaskMethodBuilder::Start<Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6>(TStateMachine&)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncTaskMethodBuilder_Start_TisU3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44_m9121BD027CA131910ADCDDCFA9EBE759B4E9AD46_gshared (AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* __this, U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* ___0_stateMachine, const RuntimeMethod* method) ;
// System.Void System.Func`4<System.Object,System.Object,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>::.ctor(System.Object,System.IntPtr)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Func_4__ctor_m2503BC75303715E496DB44214F2078E78893DA2A_gshared (Func_4_tEF7A1074DE2C6C762CE8411E0D464E5C540B5300* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Marshaller`1<System.Object>::.ctor(System.Func`2<T,System.Byte[]>,System.Func`2<System.Byte[],T>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Marshaller_1__ctor_m97D29FA2A4D6C7850DE1B906DCE71D9B38AE6D7A_gshared (Marshaller_1_t5AF2761BFFFBF2DF3FEBBAA0BEEE874BA4435EC4* __this, Func_2_tAF06B745A36D23F105F8D17F16BC771E5D76E051* ___0_serializer, Func_2_tEE2E132F8908935795FF1561B1E774E68C1BFEFF* ___1_deserializer, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void List_1__ctor_m7F078BB342729BDF11327FD89D7872265328F690_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, const RuntimeMethod* method) ;
// System.Int32 System.Collections.Generic.List`1<System.Object>::get_Count()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t List_1_get_Count_m4407E4C389F22B8CEC282C15D56516658746C383_gshared_inline (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, const RuntimeMethod* method) ;
// T System.Collections.Generic.List`1<System.Object>::get_Item(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* List_1_get_Item_m33561245D64798C2AB07584C0EC4F240E4839A38_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, int32_t ___0_index, const RuntimeMethod* method) ;
// System.Int32 System.Collections.Generic.List`1<System.Object>::IndexOf(T)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t List_1_IndexOf_m378F61BA812B79DEE58D86FE8AA9F20E3FC7D85F_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, RuntimeObject* ___0_item, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::Insert(System.Int32,T)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void List_1_Insert_m9C9559248941FED50561DB029D55DF08DEF3B094_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, int32_t ___0_index, RuntimeObject* ___1_item, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::RemoveAt(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void List_1_RemoveAt_m54F62297ADEE4D4FDA697F49ED807BF901201B54_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, int32_t ___0_index, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::set_Item(System.Int32,T)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void List_1_set_Item_m3C58DBC69A321AF2826595584FF3E9F43C07EA56_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, int32_t ___0_index, RuntimeObject* ___1_value, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::Add(T)
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void List_1_Add_mEBCF994CC3814631017F46A387B1A192ED6C85C7_gshared_inline (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, RuntimeObject* ___0_item, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::Clear()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void List_1_Clear_m16C1F2C61FED5955F10EB36BC1CB2DF34B128994_gshared_inline (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, const RuntimeMethod* method) ;
// System.Boolean System.Collections.Generic.List`1<System.Object>::Contains(T)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool List_1_Contains_m4C9139C2A6B23E9343D3F87807B32C6E2CFE660D_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, RuntimeObject* ___0_item, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<System.Object>::CopyTo(T[],System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void List_1_CopyTo_m1210D480F3EF1C250A53086148C1204B7A99B931_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* ___0_array, int32_t ___1_arrayIndex, const RuntimeMethod* method) ;
// System.Boolean System.Collections.Generic.List`1<System.Object>::Remove(T)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool List_1_Remove_m4DFA48F4CEB9169601E75FC28517C5C06EFA5AD7_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, RuntimeObject* ___0_item, const RuntimeMethod* method) ;
// System.Collections.Generic.List`1/Enumerator<T> System.Collections.Generic.List`1<System.Object>::GetEnumerator()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Enumerator_t9473BAB568A27E2339D48C1F91319E0F6D244D7A List_1_GetEnumerator_mD8294A7FA2BEB1929487127D476F8EC1CDC23BFC_gshared (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.Dictionary`2<System.Object,System.Object>::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329_gshared (Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* __this, const RuntimeMethod* method) ;
// System.Collections.Generic.IEnumerable`1<TSource> System.Linq.Enumerable::Reverse<System.Object>(System.Collections.Generic.IEnumerable`1<TSource>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* Enumerable_Reverse_TisRuntimeObject_mF7B0A079765A37716D52A158EBD61E57C390CBC9_gshared (RuntimeObject* ___0_source, const RuntimeMethod* method) ;

// System.Void System.Attribute::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2 (Attribute_tFDA8EFEFB0711976D22474794576DAF28F7440AA* __this, const RuntimeMethod* method) ;
// System.Void System.Object::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2 (RuntimeObject* __this, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.String>(T)
inline String_t* GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9 (String_t* ___0_reference, const RuntimeMethod* method)
{
	return ((  String_t* (*) (String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// System.Void Grpc.Core.AsyncCallState::.ctor(System.Func`2<System.Object,System.Threading.Tasks.Task`1<Grpc.Core.Metadata>>,System.Func`2<System.Object,Grpc.Core.Status>,System.Func`2<System.Object,Grpc.Core.Metadata>,System.Action`1<System.Object>,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncCallState__ctor_m2761326419B34F462224F8BD6981EA747039F704 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* ___0_responseHeadersAsync, Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* ___1_getStatusFunc, Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* ___2_getTrailersFunc, Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* ___3_disposeAction, RuntimeObject* ___4_callbackState, const RuntimeMethod* method) ;
// System.Void Grpc.Core.AsyncCallState::.ctor(System.Threading.Tasks.Task`1<Grpc.Core.Metadata>,System.Func`1<Grpc.Core.Status>,System.Func`1<Grpc.Core.Metadata>,System.Action)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncCallState__ctor_m257BB7A5A2835AD2A035D5ED6A325A2CAA3B46F0 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* ___0_responseHeadersAsync, Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0* ___1_getStatusFunc, Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C* ___2_getTrailersFunc, Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07* ___3_disposeAction, const RuntimeMethod* method) ;
// TResult System.Func`2<System.Object,System.Threading.Tasks.Task`1<Grpc.Core.Metadata>>::Invoke(T)
inline Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* Func_2_Invoke_m5B8E3DC21ABC9447271E3B648193324267F53679_inline (Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* __this, RuntimeObject* ___0_arg, const RuntimeMethod* method)
{
	return ((  Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* (*) (Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974*, RuntimeObject*, const RuntimeMethod*))Func_2_Invoke_mDBA25DA5DA5B7E056FB9B026AF041F1385FB58A9_gshared_inline)(__this, ___0_arg, method);
}
// System.Threading.Tasks.Task`1<Grpc.Core.Metadata> Grpc.Core.AsyncCallState::ResponseHeadersAsync()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* AsyncCallState_ResponseHeadersAsync_m700324CB96BA18E2DB0FFDA1FC83C048EA03CEA5 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) ;
// TResult System.Func`1<Grpc.Core.Status>::Invoke()
inline Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 Func_1_Invoke_m349A2DF2DF3672A6D87AA4E5E86E8AF54B24A956_inline (Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0* __this, const RuntimeMethod* method)
{
	return ((  Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 (*) (Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0*, const RuntimeMethod*))Func_1_Invoke_m349A2DF2DF3672A6D87AA4E5E86E8AF54B24A956_gshared_inline)(__this, method);
}
// TResult System.Func`2<System.Object,Grpc.Core.Status>::Invoke(T)
inline Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 Func_2_Invoke_m497B8CC89F1330EB7265E70F432347348B580F14_inline (Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* __this, RuntimeObject* ___0_arg, const RuntimeMethod* method)
{
	return ((  Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 (*) (Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2*, RuntimeObject*, const RuntimeMethod*))Func_2_Invoke_m497B8CC89F1330EB7265E70F432347348B580F14_gshared_inline)(__this, ___0_arg, method);
}
// Grpc.Core.Status Grpc.Core.AsyncCallState::GetStatus()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 AsyncCallState_GetStatus_mB33352422BEB5CEE4C03E69A3E68C1EC01B8D265 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) ;
// TResult System.Func`1<Grpc.Core.Metadata>::Invoke()
inline Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* Func_1_Invoke_m3E136C280A87E956C5A2B8EC105691F5AAAB293D_inline (Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C* __this, const RuntimeMethod* method)
{
	return ((  Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* (*) (Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C*, const RuntimeMethod*))Func_1_Invoke_m1412272198DFA4066C83206E5B43353AF10A2EEE_gshared_inline)(__this, method);
}
// TResult System.Func`2<System.Object,Grpc.Core.Metadata>::Invoke(T)
inline Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* Func_2_Invoke_m1EDA519D53C4FE8082F91071B2A383E9788FFA4B_inline (Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* __this, RuntimeObject* ___0_arg, const RuntimeMethod* method)
{
	return ((  Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* (*) (Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A*, RuntimeObject*, const RuntimeMethod*))Func_2_Invoke_mDBA25DA5DA5B7E056FB9B026AF041F1385FB58A9_gshared_inline)(__this, ___0_arg, method);
}
// Grpc.Core.Metadata Grpc.Core.AsyncCallState::GetTrailers()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* AsyncCallState_GetTrailers_m5E344A799E41C38135EBB3B5BC179E11BFAF46D5 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) ;
// System.Void System.Action`1<System.Object>::Invoke(T)
inline void Action_1_Invoke_mF2422B2DD29F74CE66F791C3F68E288EC7C3DB9E_inline (Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* __this, RuntimeObject* ___0_obj, const RuntimeMethod* method)
{
	((  void (*) (Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87*, RuntimeObject*, const RuntimeMethod*))Action_1_Invoke_mF2422B2DD29F74CE66F791C3F68E288EC7C3DB9E_gshared_inline)(__this, ___0_obj, method);
}
// System.Void System.Action::Invoke()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void Action_Invoke_m7126A54DACA72B845424072887B5F3A51FC3808E_inline (Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.AsyncCallState::Dispose()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncCallState_Dispose_m92ACEB8E0BB514EE0DF548E9CF5F8903236E5A70 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>>(T)
inline Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* GrpcPreconditions_CheckNotNull_TisDictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50_m7ED3349BDCE75D560CFB3D131E416BFB92D48222 (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* ___0_reference, const RuntimeMethod* method)
{
	return ((  Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* (*) (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// System.Collections.Generic.IEnumerable`1<TResult> System.Linq.Enumerable::Empty<Grpc.Core.AuthProperty>()
inline RuntimeObject* Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_inline (const RuntimeMethod* method)
{
	return ((  RuntimeObject* (*) (const RuntimeMethod*))Enumerable_Empty_TisRuntimeObject_mA90CDE158774C34A28C07CEEA9E9EA2A61618238_gshared_inline)(method);
}
// TValue System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>::get_Item(TKey)
inline List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A* Dictionary_2_get_Item_mCC86C07EAC13D2EC7C63C99AB267F4A9B0BCBA7E (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* __this, String_t* ___0_key, const RuntimeMethod* method)
{
	return ((  List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A* (*) (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50*, String_t*, const RuntimeMethod*))Dictionary_2_get_Item_m4AAAECBE902A211BF2126E6AFA280AEF73A3E0D6_gshared)(__this, ___0_key, method);
}
// System.Collections.Generic.Dictionary`2/ValueCollection<TKey,TValue> System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>::get_Values()
inline ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010* Dictionary_2_get_Values_m23DB66E8602DED9744E45784C6E3A2F2C8F513D4 (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* __this, const RuntimeMethod* method)
{
	return ((  ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010* (*) (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50*, const RuntimeMethod*))Dictionary_2_get_Values_mA0C01DEA55329E55380E96BBD04D4D228B437EC5_gshared)(__this, method);
}
// System.Void System.Func`2<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>,System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty>>::.ctor(System.Object,System.IntPtr)
inline void Func_2__ctor_m53DF15B454B7C7C541DE99174419A101E140AB51 (Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method)
{
	((  void (*) (Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A*, RuntimeObject*, intptr_t, const RuntimeMethod*))Func_2__ctor_m7F8A01C0B02BC1D4063F4EB1E817F7A48562A398_gshared)(__this, ___0_object, ___1_method, method);
}
// System.Collections.Generic.IEnumerable`1<TResult> System.Linq.Enumerable::SelectMany<System.Collections.Generic.List`1<Grpc.Core.AuthProperty>,Grpc.Core.AuthProperty>(System.Collections.Generic.IEnumerable`1<TSource>,System.Func`2<TSource,System.Collections.Generic.IEnumerable`1<TResult>>)
inline RuntimeObject* Enumerable_SelectMany_TisList_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mEA8E736006D5A38482763344EB5EF9B911A7DDFA (RuntimeObject* ___0_source, Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* ___1_selector, const RuntimeMethod* method)
{
	return ((  RuntimeObject* (*) (RuntimeObject*, Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A*, const RuntimeMethod*))Enumerable_SelectMany_TisRuntimeObject_TisRuntimeObject_mC89216034DEE8779F1AC2D0A984C0ADE855BED00_gshared)(___0_source, ___1_selector, method);
}
// System.Boolean System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>::TryGetValue(TKey,TValue&)
inline bool Dictionary_2_TryGetValue_m06D597E171BEBB18AC362AE27BBD7BEE0A0C4F7F (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* __this, String_t* ___0_key, List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A** ___1_value, const RuntimeMethod* method)
{
	return ((  bool (*) (Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50*, String_t*, List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A**, const RuntimeMethod*))Dictionary_2_TryGetValue_mD15380A4ED7CDEE99EA45881577D26BA9CE1B849_gshared)(__this, ___0_key, ___1_value, method);
}
// System.Void Grpc.Core.AuthContext/<>c::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CU3Ec__ctor_m6285DC63E7911C62ED2A49AB2A494C45FCEF0C31 (U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677* __this, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.Byte[]>(T)
inline ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059 (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___0_reference, const RuntimeMethod* method)
{
	return ((  ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* (*) (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// System.Void System.Buffer::BlockCopy(System.Array,System.Int32,System.Array,System.Int32,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Buffer_BlockCopy_m2F7BC0C5BA97C500E3F87D5008718F797E02B358 (RuntimeArray* ___0_src, int32_t ___1_srcOffset, RuntimeArray* ___2_dst, int32_t ___3_dstOffset, int32_t ___4_count, const RuntimeMethod* method) ;
// System.Void Grpc.Core.AuthProperty::.ctor(System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AuthProperty__ctor_mDEECB7A796ACFFF79FE6EAAD654310264613386E (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* __this, String_t* ___0_name, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_valueBytes, const RuntimeMethod* method) ;
// System.Text.Encoding System.Text.Encoding::get_UTF8()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* Encoding_get_UTF8_m9FA98A53CE96FD6D02982625C5246DD36C1235C9 (const RuntimeMethod* method) ;
// System.Void Grpc.Core.CallCredentials/CompositeCallCredentials::.ctor(Grpc.Core.CallCredentials[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CompositeCallCredentials__ctor_m9900E80BCCAC873F8E1363C3FF4075DC81CF9E0E (CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E* __this, CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* ___0_credentials, const RuntimeMethod* method) ;
// System.Void Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials::.ctor(Grpc.Core.AsyncAuthInterceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncAuthInterceptorCredentials__ctor_m71EBDE4E90210DDFC681326E27CA4EF245769D69 (AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862* __this, AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* ___0_interceptor, const RuntimeMethod* method) ;
// System.Void Grpc.Core.CallCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CallCredentials__ctor_m88A74796A15DA1B237649AE2F738E8F4834B7141 (CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Utils.GrpcPreconditions::CheckArgument(System.Boolean,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497 (bool ___0_condition, String_t* ___1_errorMessage, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<Grpc.Core.CallCredentials>::.ctor(System.Collections.Generic.IEnumerable`1<T>)
inline void List_1__ctor_m5488D09159AC2D56E9F9779F44ADF0DD0E684049 (List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1* __this, RuntimeObject* ___0_collection, const RuntimeMethod* method)
{
	((  void (*) (List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1*, RuntimeObject*, const RuntimeMethod*))List_1__ctor_mE097DBD72433D1AFC11733F5678602603A756424_gshared)(__this, ___0_collection, method);
}
// System.Collections.ObjectModel.ReadOnlyCollection`1<T> System.Collections.Generic.List`1<Grpc.Core.CallCredentials>::AsReadOnly()
inline ReadOnlyCollection_1_tC203378F06B8F145A48BFB0A1546E72C1B700445* List_1_AsReadOnly_m730830764F278CFE6CD6A5E39836BFD8BED58AED (List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1* __this, const RuntimeMethod* method)
{
	return ((  ReadOnlyCollection_1_tC203378F06B8F145A48BFB0A1546E72C1B700445* (*) (List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1*, const RuntimeMethod*))List_1_AsReadOnly_m69D18D2353099DC61F87AD42FF87F25B1A4D299B_gshared)(__this, method);
}
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.AsyncAuthInterceptor>(T)
inline AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* GrpcPreconditions_CheckNotNull_TisAsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968_m0512535AE0D48827C37B1D3A08E71D0AE57FEE20 (AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* ___0_reference, const RuntimeMethod* method)
{
	return ((  AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* (*) (AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// System.Void Grpc.Core.CallOptions::.ctor(Grpc.Core.Metadata,System.Nullable`1<System.DateTime>,System.Threading.CancellationToken,Grpc.Core.WriteOptions,Grpc.Core.ContextPropagationToken,Grpc.Core.CallCredentials)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CallOptions__ctor_m3CA2A09D88B01048E1C33BAA895BD8133F098CA2 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_headers, Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC ___1_deadline, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ___2_cancellationToken, WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___3_writeOptions, ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___4_propagationToken, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___5_credentials, const RuntimeMethod* method) ;
// Grpc.Core.Metadata Grpc.Core.CallOptions::get_Headers()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* CallOptions_get_Headers_mC83DA74FDC549907063A89E8E3589E244EE244BB_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// System.Nullable`1<System.DateTime> Grpc.Core.CallOptions::get_Deadline()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC CallOptions_get_Deadline_m2B9B9FB43004CBA8DE9B7BFE125991B4FB24EEDA_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// System.Threading.CancellationToken Grpc.Core.CallOptions::get_CancellationToken()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED CallOptions_get_CancellationToken_m26B31F9F5DDE686ED39260681F29301AA878068B_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// Grpc.Core.WriteOptions Grpc.Core.CallOptions::get_WriteOptions()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* CallOptions_get_WriteOptions_m7F6A4E257CA22FD8E4D3FA0633DACABA5B95817E_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// Grpc.Core.ContextPropagationToken Grpc.Core.CallOptions::get_PropagationToken()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* CallOptions_get_PropagationToken_m6FAE4454FD0AC717C9DFA54255ACCDA9D834C522_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// Grpc.Core.CallCredentials Grpc.Core.CallOptions::get_Credentials()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* CallOptions_get_Credentials_m36616F8A973A7EC538A623897CC7A0A832BACF9F_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// System.Boolean Grpc.Core.CallOptions::get_IsWaitForReady()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool CallOptions_get_IsWaitForReady_m5F54CD453B786EA853E52B700D7D1BCD403DA4C4 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// Grpc.Core.Internal.CallFlags Grpc.Core.CallOptions::get_Flags()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t CallOptions_get_Flags_mA4C453704C2AC0C8E04BE00C588ACE4E1F8CC969_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) ;
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithHeaders(Grpc.Core.Metadata)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithHeaders_m5AEB13430BD0210D3BC3D247015728F3417FF82B (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_headers, const RuntimeMethod* method) ;
// System.Void System.Nullable`1<System.DateTime>::.ctor(T)
inline void Nullable_1__ctor_mB17304720EA19F5469A4883827F53A75FEB492CF (Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC* __this, DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___0_value, const RuntimeMethod* method)
{
	((  void (*) (Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC*, DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D, const RuntimeMethod*))Nullable_1__ctor_mB17304720EA19F5469A4883827F53A75FEB492CF_gshared)(__this, ___0_value, method);
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithDeadline(System.DateTime)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithDeadline_mA865862C3CF382BC7493F8A4F7102476CF61A44A (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___0_deadline, const RuntimeMethod* method) ;
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithCancellationToken(System.Threading.CancellationToken)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithCancellationToken_m1F875E2F1757A6AE7E1205096C33DE8F1E795576 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ___0_cancellationToken, const RuntimeMethod* method) ;
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithWriteOptions(Grpc.Core.WriteOptions)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithWriteOptions_mAB5238A794F9B29D1F1D8222C1C4C233812FD1B8 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___0_writeOptions, const RuntimeMethod* method) ;
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithPropagationToken(Grpc.Core.ContextPropagationToken)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithPropagationToken_m483DBA0E2928ACEB07184F87ACDA7F1E98C16248 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___0_propagationToken, const RuntimeMethod* method) ;
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithCredentials(Grpc.Core.CallCredentials)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithCredentials_m7F68A03E4A1A680D956A13163E4DD90172F80D4E (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___0_credentials, const RuntimeMethod* method) ;
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithFlags(Grpc.Core.Internal.CallFlags)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithFlags_mBF19C96B91674EF7022CDEDF071C21788B231CB5 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, int32_t ___0_flags, const RuntimeMethod* method) ;
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithWaitForReady(System.Boolean)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithWaitForReady_mAD0AD13D6E4D0289DCDB2F205A6074E7454D6A71 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, bool ___0_waitForReady, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.String>(T,System.String)
inline String_t* GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B (String_t* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  String_t* (*) (String_t*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// System.Runtime.CompilerServices.AsyncTaskMethodBuilder System.Runtime.CompilerServices.AsyncTaskMethodBuilder::Create()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06 AsyncTaskMethodBuilder_Create_m24F3F260A79CA2B9EC4F1F15693A33F5EC58735A (const RuntimeMethod* method) ;
// System.Void System.Runtime.CompilerServices.AsyncTaskMethodBuilder::Start<Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6>(TStateMachine&)
inline void AsyncTaskMethodBuilder_Start_TisU3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44_m9121BD027CA131910ADCDDCFA9EBE759B4E9AD46 (AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* __this, U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* ___0_stateMachine, const RuntimeMethod* method)
{
	((  void (*) (AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06*, U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44*, const RuntimeMethod*))AsyncTaskMethodBuilder_Start_TisU3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44_m9121BD027CA131910ADCDDCFA9EBE759B4E9AD46_gshared)(__this, ___0_stateMachine, method);
}
// System.Threading.Tasks.Task System.Runtime.CompilerServices.AsyncTaskMethodBuilder::get_Task()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncTaskMethodBuilder_get_Task_mE9125D5F8B96F26D1BA5A5347ED82669521C7F9E (AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* __this, const RuntimeMethod* method) ;
// System.Void System.Runtime.CompilerServices.AsyncTaskMethodBuilder::SetException(System.Exception)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncTaskMethodBuilder_SetException_mBE41863F0571E0177A15731294087DE45E1FC10B (AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* __this, Exception_t* ___0_exception, const RuntimeMethod* method) ;
// System.Void System.Runtime.CompilerServices.AsyncTaskMethodBuilder::SetResult()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncTaskMethodBuilder_SetResult_m76D8B84F0068257C1823B1200B00E58E0C8DDDDE (AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6::MoveNext()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CShutdownAsyncCoreU3Ed__6_MoveNext_m4018328719353F183D6B961B3EA66CF8E0E48304 (U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* __this, const RuntimeMethod* method) ;
// System.Void System.Runtime.CompilerServices.AsyncTaskMethodBuilder::SetStateMachine(System.Runtime.CompilerServices.IAsyncStateMachine)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncTaskMethodBuilder_SetStateMachine_mE52B5B6B076025592A7AB462E3D26FA434AEB795 (AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* __this, RuntimeObject* ___0_stateMachine, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6::SetStateMachine(System.Runtime.CompilerServices.IAsyncStateMachine)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CShutdownAsyncCoreU3Ed__6_SetStateMachine_m133828B5FD4D7C31A38471B78C3A0B4799E98864 (U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* __this, RuntimeObject* ___0_stateMachine, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ChannelCredentials/CompositeChannelCredentials::.ctor(Grpc.Core.ChannelCredentials,Grpc.Core.CallCredentials)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CompositeChannelCredentials__ctor_mECB83F30B1E805ACD5F037105A1C793077ED0E86 (CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740* __this, ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ___0_channelCredentials, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___1_callCredentials, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ChannelCredentials/InsecureCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void InsecureCredentials__ctor_mBCDF914314FAAF1F79AD2D0F41C301CC4E315C12 (InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.SslCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SslCredentials__ctor_mB827017C7E77D62BBEACEFE1C7455AB9D619DFB3 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ChannelCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ChannelCredentials__ctor_m5BAB665085468AD860D966A9B09538F491150849 (ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* __this, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.ChannelCredentials>(T)
inline ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* GrpcPreconditions_CheckNotNull_TisChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_m13ACE0719712FCF60B60D0CFFBE35BAC3F9E1DB4 (ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ___0_reference, const RuntimeMethod* method)
{
	return ((  ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* (*) (ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.CallCredentials>(T)
inline CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* GrpcPreconditions_CheckNotNull_TisCallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466_mB0D2D758B2EE1819303789F98C090EA881FE012E (CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___0_reference, const RuntimeMethod* method)
{
	return ((  CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* (*) (CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// System.Type System.Object::GetType()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Type_t* Object_GetType_mE10A8FC1E57F3DF29972CCBC026C2DC3942263B3 (RuntimeObject* __this, const RuntimeMethod* method) ;
// System.String System.String::Format(System.String,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* String_Format_mA8DBB4C2516B9723C5A41E6CB1E2FAF4BBE96DD8 (String_t* ___0_format, RuntimeObject* ___1_arg0, const RuntimeMethod* method) ;
// System.Void System.ArgumentException::.ctor(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ArgumentException__ctor_m026938A67AF9D36BB7ED27F80425D7194B514465 (ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263* __this, String_t* ___0_message, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Internal.UnimplementedCallInvoker::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void UnimplementedCallInvoker__ctor_m56CA5FABA47729295B56022F92B907D32DBFA0AD (UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ClientBase::.ctor(Grpc.Core.CallInvoker)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBase__ctor_mBB9AD81C869A8006A8C422E7AD83AE0ADBD573DB (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_callInvoker, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.ClientBase/ClientBaseConfiguration>(T,System.String)
inline ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* GrpcPreconditions_CheckNotNull_TisClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_m7EB6EB36D0C253043670DC0A19000775E31CB16C (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* (*) (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// Grpc.Core.CallInvoker Grpc.Core.ClientBase/ClientBaseConfiguration::CreateDecoratedCallInvoker()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ClientBaseConfiguration_CreateDecoratedCallInvoker_mE305C7894FB598F137DC120396393B2AA6A5E7B6 (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ClientBase/ClientBaseConfiguration::.ctor(Grpc.Core.CallInvoker,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBaseConfiguration__ctor_m3BA0C26F2E8F95872BCDDCEAAD302B98CD4BE33F (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* __this, CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_undecoratedCallInvoker, String_t* ___1_host, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ClientBase::.ctor(Grpc.Core.ClientBase/ClientBaseConfiguration)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBase__ctor_mA7F300DBD8471A62FCD6475413DFF3B7783B6D70 (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* ___0_configuration, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.CallInvoker>(T)
inline CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_mAD4DE615EDB070D206521A1845F088B4ABAC8966 (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_reference, const RuntimeMethod* method)
{
	return ((  CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* (*) (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// System.Void System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>::.ctor(System.Object,System.IntPtr)
inline void Func_4__ctor_mC4ADA479232073776D307BC5CDB7E2B13E82A910 (Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method)
{
	((  void (*) (Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20*, RuntimeObject*, intptr_t, const RuntimeMethod*))Func_4__ctor_m2503BC75303715E496DB44214F2078E78893DA2A_gshared)(__this, ___0_object, ___1_method, method);
}
// System.Void Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInterceptor::.ctor(System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBaseConfigurationInterceptor__ctor_m0DA8B4A915C09AACF2B1C2EB819505EC59A06583 (ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425* __this, Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* ___0_interceptor, const RuntimeMethod* method) ;
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.CallInvokerExtensions::Intercept(Grpc.Core.CallInvoker,Grpc.Core.Interceptors.Interceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* CallInvokerExtensions_Intercept_m415B574E557536E895A7DAC8AE5B3B14026B135A (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* ___1_interceptor, const RuntimeMethod* method) ;
// System.Void Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo::.ctor(System.String,Grpc.Core.CallOptions)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBaseConfigurationInfo__ctor_m7942448EB246ED72C3F9B3D64896727E92D3C679 (ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9* __this, String_t* ___0_host, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E ___1_callOptions, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Interceptors.Interceptor::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Interceptor__ctor_m0A48ED17B15161D356A95F546D1AADB995725169 (Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* __this, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>>(T,System.String)
inline Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* GrpcPreconditions_CheckNotNull_TisFunc_4_t737C862B437880FF1CD553B949328C8BCF76DD20_m5CD271FF760C0D77688A24811BE9FF6518D3090D (Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* (*) (Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// System.Void Grpc.Core.ContextPropagationOptions::.ctor(System.Boolean,System.Boolean)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ContextPropagationOptions__ctor_m96E390576401E7BA84886C7E6F3BDD38CA9C3E4A (ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* __this, bool ___0_propagateDeadline, bool ___1_propagateCancellation, const RuntimeMethod* method) ;
// System.Void System.NotImplementedException::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void NotImplementedException__ctor_mDAB47BC6BD0E342E8F2171E5CABE3E67EA049F1C (NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8* __this, const RuntimeMethod* method) ;
// System.Void System.Func`2<System.String,System.Byte[]>::.ctor(System.Object,System.IntPtr)
inline void Func_2__ctor_m573F485A6595759C50F07B6051007D317FE3E27F (Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method)
{
	((  void (*) (Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C*, RuntimeObject*, intptr_t, const RuntimeMethod*))Func_2__ctor_m7F8A01C0B02BC1D4063F4EB1E817F7A48562A398_gshared)(__this, ___0_object, ___1_method, method);
}
// System.Void System.Func`2<System.Byte[],System.String>::.ctor(System.Object,System.IntPtr)
inline void Func_2__ctor_m0779F5BE8A3C3178996AC0BE54575D71FB4D35B4 (Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method)
{
	((  void (*) (Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF*, RuntimeObject*, intptr_t, const RuntimeMethod*))Func_2__ctor_m7F8A01C0B02BC1D4063F4EB1E817F7A48562A398_gshared)(__this, ___0_object, ___1_method, method);
}
// System.Void Grpc.Core.Marshaller`1<System.String>::.ctor(System.Func`2<T,System.Byte[]>,System.Func`2<System.Byte[],T>)
inline void Marshaller_1__ctor_mF20CB78867ABA38F189F7DCE7D6DCA50C197A6DE (Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04* __this, Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C* ___0_serializer, Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF* ___1_deserializer, const RuntimeMethod* method)
{
	((  void (*) (Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04*, Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C*, Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF*, const RuntimeMethod*))Marshaller_1__ctor_m97D29FA2A4D6C7850DE1B906DCE71D9B38AE6D7A_gshared)(__this, ___0_serializer, ___1_deserializer, method);
}
// System.Void System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::.ctor()
inline void List_1__ctor_mC61673FF37950228AD28C7E2D5798F7E822165FE (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, const RuntimeMethod* method)
{
	((  void (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, const RuntimeMethod*))List_1__ctor_m7F078BB342729BDF11327FD89D7872265328F690_gshared)(__this, method);
}
// System.Int32 System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::get_Count()
inline int32_t List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_inline (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, const RuntimeMethod* method)
{
	return ((  int32_t (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, const RuntimeMethod*))List_1_get_Count_m4407E4C389F22B8CEC282C15D56516658746C383_gshared_inline)(__this, method);
}
// T System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::get_Item(System.Int32)
inline Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9 (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, int32_t ___0_index, const RuntimeMethod* method)
{
	return ((  Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, int32_t, const RuntimeMethod*))List_1_get_Item_m33561245D64798C2AB07584C0EC4F240E4839A38_gshared)(__this, ___0_index, method);
}
// System.Boolean Grpc.Core.Metadata/Entry::KeyEqualsIgnoreCase(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_KeyEqualsIgnoreCase_m5765029234BADCFDF40F5E1BD2E4CF3313D10A27 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, const RuntimeMethod* method) ;
// Grpc.Core.Metadata/Entry Grpc.Core.Metadata::Get(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* Metadata_Get_mAB88DB01CE0B1319FB4E239A795DFD6B1CF6F506 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, String_t* ___0_key, const RuntimeMethod* method) ;
// System.String Grpc.Core.Metadata/Entry::get_Value()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Entry_get_Value_m1C0D0600AFC16B36093ED5B021335B952C4FD696 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) ;
// System.Byte[] Grpc.Core.Metadata/Entry::get_ValueBytes()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* Entry_get_ValueBytes_m8952343C41FE19A9A5190F2C15D71BDB3CF8F5B9 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Metadata/<GetAll>d__11::.ctor(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CGetAllU3Ed__11__ctor_m35967E7C5537A498542DC7F352BD828EA734D4A0 (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, int32_t ___0_U3CU3E1__state, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Metadata/Entry::.ctor(System.String,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Entry__ctor_m06E649C4EA82D12900A074292784F649C3D0538D (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, String_t* ___1_value, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Metadata::Add(Grpc.Core.Metadata/Entry)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_Add_m189C162DF2490839465923F96C443BA1189D8B2A (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Metadata/Entry::.ctor(System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Entry__ctor_m7F675669248FB0A305E5B705BEC9550446AF5B0A (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_valueBytes, const RuntimeMethod* method) ;
// System.Int32 System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::IndexOf(T)
inline int32_t List_1_IndexOf_m098D47D60BAF4C30BB0E8FF3CF8EF49093D6FD2B (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method)
{
	return ((  int32_t (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*, const RuntimeMethod*))List_1_IndexOf_m378F61BA812B79DEE58D86FE8AA9F20E3FC7D85F_gshared)(__this, ___0_item, method);
}
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.Metadata/Entry>(T)
inline Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_reference, const RuntimeMethod* method)
{
	return ((  Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* (*) (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// System.Void Grpc.Core.Metadata::CheckWriteable()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::Insert(System.Int32,T)
inline void List_1_Insert_m383E4FA9E7D234C2BFAC3B41E038B15CFA80AF05 (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, int32_t ___0_index, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___1_item, const RuntimeMethod* method)
{
	((  void (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, int32_t, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*, const RuntimeMethod*))List_1_Insert_m9C9559248941FED50561DB029D55DF08DEF3B094_gshared)(__this, ___0_index, ___1_item, method);
}
// System.Void System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::RemoveAt(System.Int32)
inline void List_1_RemoveAt_m76954D81D216842D8A3ADD420E6588EABFA34A6A (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, int32_t ___0_index, const RuntimeMethod* method)
{
	((  void (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, int32_t, const RuntimeMethod*))List_1_RemoveAt_m54F62297ADEE4D4FDA697F49ED807BF901201B54_gshared)(__this, ___0_index, method);
}
// System.Void System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::set_Item(System.Int32,T)
inline void List_1_set_Item_mDBC1AAC216D70CE8B5E8CB3AAF3083E65247CFC3 (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, int32_t ___0_index, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___1_value, const RuntimeMethod* method)
{
	((  void (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, int32_t, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*, const RuntimeMethod*))List_1_set_Item_m3C58DBC69A321AF2826595584FF3E9F43C07EA56_gshared)(__this, ___0_index, ___1_value, method);
}
// System.Void System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::Add(T)
inline void List_1_Add_m8626F0812751DA3B7FF22FC178565F1326A68CD0_inline (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method)
{
	((  void (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*, const RuntimeMethod*))List_1_Add_mEBCF994CC3814631017F46A387B1A192ED6C85C7_gshared_inline)(__this, ___0_item, method);
}
// System.Void System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::Clear()
inline void List_1_Clear_mAAD225470B9D453E533818038DA07BC1494513DC_inline (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, const RuntimeMethod* method)
{
	((  void (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, const RuntimeMethod*))List_1_Clear_m16C1F2C61FED5955F10EB36BC1CB2DF34B128994_gshared_inline)(__this, method);
}
// System.Boolean System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::Contains(T)
inline bool List_1_Contains_m3750DD90BC38CC246D898710BA06F7F266DEFB78 (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method)
{
	return ((  bool (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*, const RuntimeMethod*))List_1_Contains_m4C9139C2A6B23E9343D3F87807B32C6E2CFE660D_gshared)(__this, ___0_item, method);
}
// System.Void System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::CopyTo(T[],System.Int32)
inline void List_1_CopyTo_m6CA081F6251F594F64C73BCB3B6915259FB42903 (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4* ___0_array, int32_t ___1_arrayIndex, const RuntimeMethod* method)
{
	((  void (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4*, int32_t, const RuntimeMethod*))List_1_CopyTo_m1210D480F3EF1C250A53086148C1204B7A99B931_gshared)(__this, ___0_array, ___1_arrayIndex, method);
}
// System.Boolean System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::Remove(T)
inline bool List_1_Remove_mBE506315950A1EA3F5351D3E7A9216015D68A6FA (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method)
{
	return ((  bool (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*, const RuntimeMethod*))List_1_Remove_m4DFA48F4CEB9169601E75FC28517C5C06EFA5AD7_gshared)(__this, ___0_item, method);
}
// System.Collections.Generic.List`1/Enumerator<T> System.Collections.Generic.List`1<Grpc.Core.Metadata/Entry>::GetEnumerator()
inline Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6 List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101 (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* __this, const RuntimeMethod* method)
{
	return ((  Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6 (*) (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*, const RuntimeMethod*))List_1_GetEnumerator_mD8294A7FA2BEB1929487127D476F8EC1CDC23BFC_gshared)(__this, method);
}
// System.Void Grpc.Core.Utils.GrpcPreconditions::CheckState(System.Boolean,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void GrpcPreconditions_CheckState_mCDE04614CE69128AF99EE030B5F83D386FDEC375 (bool ___0_condition, String_t* ___1_errorMessage, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Metadata::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata__ctor_mAA8F9E779B141794B1A58388BE41D32F97B0085A (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) ;
// Grpc.Core.Metadata Grpc.Core.Metadata::Freeze()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* Metadata_Freeze_m6A755BCE0A04B4A1B690D05277D90BBD1F3A4AD1 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) ;
// System.Text.Encoding System.Text.Encoding::get_ASCII()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* Encoding_get_ASCII_mCC61B512D320FD4E2E71CC0DFDF8DDF3CD215C65 (const RuntimeMethod* method) ;
// System.String Grpc.Core.Metadata/Entry::NormalizeKey(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Entry_NormalizeKey_m87C9D5E6C68C229764D85F62077CA4EA6BA3EE3C (String_t* ___0_key, const RuntimeMethod* method) ;
// System.Boolean Grpc.Core.Metadata/Entry::HasBinaryHeaderSuffix(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_HasBinaryHeaderSuffix_m7BCA7E3AAB2BE2C598460A2E8A3EEEEFDFD5DBA7 (String_t* ___0_key, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.Byte[]>(T,System.String)
inline ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_m2BEB1EC4BC44E79026DB6011E1E94018C77954EE (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* (*) (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// System.Boolean Grpc.Core.Metadata/Entry::get_IsBinary()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_get_IsBinary_m0A897895938055DB3CCBECB3B3AD72DC596D6E19 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) ;
// System.String System.String::Format(System.String,System.Object,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* String_Format_mFB7DA489BD99F4670881FF50EC017BFB0A5C0987 (String_t* ___0_format, RuntimeObject* ___1_arg0, RuntimeObject* ___2_arg1, const RuntimeMethod* method) ;
// System.Boolean System.String::Equals(System.String,System.String,System.StringComparison)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool String_Equals_mCC34895D0DB2AD440C9D8767032215BC86B5C48B (String_t* ___0_a, String_t* ___1_b, int32_t ___2_comparisonType, const RuntimeMethod* method) ;
// System.Void System.Runtime.InteropServices.Marshal::Copy(System.IntPtr,System.Byte[],System.Int32,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Marshal_Copy_mF7402FFDB520EA1B8D1C32B368DBEE4B13F1BE77 (intptr_t ___0_source, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_destination, int32_t ___2_startIndex, int32_t ___3_length, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Metadata/Entry::.ctor(System.String,System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Entry__ctor_m02C3B36E523E4E0215CB3E0A1EC30A906A6EF27E (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, String_t* ___1_value, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___2_valueBytes, const RuntimeMethod* method) ;
// System.String Grpc.Core.Api.Utils.EncodingExtensions::GetString(System.Text.Encoding,System.IntPtr,System.Int32)
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR String_t* EncodingExtensions_GetString_m5290CA29398B11EA0507EEEBE965E7E78137EEFF_inline (Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___0_encoding, intptr_t ___1_ptr, int32_t ___2_len, const RuntimeMethod* method) ;
// System.Boolean Grpc.Core.Metadata/Entry::IsValidKey(System.String,System.Boolean&)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_IsValidKey_m564FB442C0B8E35F085DAA3C3C44C400B2FE4EE9 (String_t* ___0_input, bool* ___1_isLowercase, const RuntimeMethod* method) ;
// System.String System.String::ToLowerInvariant()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* String_ToLowerInvariant_mBE32C93DE27C5353FEA3FA654FC1DDBE3D0EB0F2 (String_t* __this, const RuntimeMethod* method) ;
// System.Char System.String::get_Chars(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Il2CppChar String_get_Chars_mC49DF0CD2D3BE7BE97B3AD9C995BE3094F8E36D3 (String_t* __this, int32_t ___0_index, const RuntimeMethod* method) ;
// System.Int32 System.String::get_Length()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t String_get_Length_m42625D67623FA5CC7A44D47425CE86FB946542D2_inline (String_t* __this, const RuntimeMethod* method) ;
// System.Int32 System.Environment::get_CurrentManagedThreadId()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t Environment_get_CurrentManagedThreadId_m66483AADCCC13272EBDCD94D31D2E52603C24BDF (const RuntimeMethod* method) ;
// System.Void System.NotSupportedException::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void NotSupportedException__ctor_m1398D0CDE19B36AA3DE9392879738C1EA2439CDF (NotSupportedException_t1429765983D409BD2986508963C98D214E4EBF4A* __this, const RuntimeMethod* method) ;
// System.Collections.Generic.IEnumerator`1<Grpc.Core.Metadata/Entry> Grpc.Core.Metadata/<GetAll>d__11::System.Collections.Generic.IEnumerable<Grpc.Core.Metadata.Entry>.GetEnumerator()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* U3CGetAllU3Ed__11_System_Collections_Generic_IEnumerableU3CGrpc_Core_Metadata_EntryU3E_GetEnumerator_m2928F9F872395CA6B69AFCA12C4C0477DBACF83B (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) ;
// System.String Grpc.Core.Status::ToString()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Status_ToString_m85A7C885F4E22BC641454E680EB9AB02A8A79036 (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) ;
// System.Void Grpc.Core.RpcException::.ctor(Grpc.Core.Status,Grpc.Core.Metadata,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void RpcException__ctor_m5602C523B475531625FDDC82B574BE2558E1DF4F (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___0_status, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_trailers, String_t* ___2_message, const RuntimeMethod* method) ;
// System.Void System.Exception::.ctor(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Exception__ctor_m9B2BD92CD68916245A75109105D9071C9D430E7F (Exception_t* __this, String_t* ___0_message, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.Metadata>(T)
inline Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* GrpcPreconditions_CheckNotNull_TisMetadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_m3DDAE807CB04B4ADAC6FB166B7EC34BE2A3720AE (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_reference, const RuntimeMethod* method)
{
	return ((  Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* (*) (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8492C9FF4328E469E1F9C757F2461A803A9A981D_gshared)(___0_reference, method);
}
// Grpc.Core.StatusCode Grpc.Core.Status::get_StatusCode()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386_inline (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.Dictionary`2<System.Object,System.Object>::.ctor()
inline void Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329 (Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* __this, const RuntimeMethod* method)
{
	((  void (*) (Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA*, const RuntimeMethod*))Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329_gshared)(__this, method);
}
// System.Collections.ObjectModel.ReadOnlyCollection`1<T> System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>::AsReadOnly()
inline ReadOnlyCollection_1_tBC605066ABB00B74ED2389020F64275AA494E462* List_1_AsReadOnly_mF5D40FE5DCA9ABF87F4D97EB90234195022B9440 (List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* __this, const RuntimeMethod* method)
{
	return ((  ReadOnlyCollection_1_tBC605066ABB00B74ED2389020F64275AA494E462* (*) (List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5*, const RuntimeMethod*))List_1_AsReadOnly_m69D18D2353099DC61F87AD42FF87F25B1A4D299B_gshared)(__this, method);
}
// System.Void System.Action`1<Grpc.Core.ServiceBinderBase>::Invoke(T)
inline void Action_1_Invoke_mD511836C3522F85EE4CD5D608A1F9D0FA9C3A674_inline (Action_1_t805B4C224C6B55AF87988396EE027CA269CD12EE* __this, ServiceBinderBase_tFB0E641375E302B3D9DEC054D9D9BA8197FC104A* ___0_obj, const RuntimeMethod* method)
{
	((  void (*) (Action_1_t805B4C224C6B55AF87988396EE027CA269CD12EE*, ServiceBinderBase_tFB0E641375E302B3D9DEC054D9D9BA8197FC104A*, const RuntimeMethod*))Action_1_Invoke_mF2422B2DD29F74CE66F791C3F68E288EC7C3DB9E_gshared_inline)(__this, ___0_obj, method);
}
// System.Void Grpc.Core.ServerServiceDefinition/Builder::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Builder__ctor_m93E6C2BE407A34F963268933CB7D6FEF59D9732E (Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32* __this, const RuntimeMethod* method) ;
// System.Void System.Collections.Generic.Dictionary`2<System.String,System.Object>::.ctor()
inline void Dictionary_2__ctor_mC4F3DF292BAD88F4BF193C49CD689FAEBC4570A9 (Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710* __this, const RuntimeMethod* method)
{
	((  void (*) (Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710*, const RuntimeMethod*))Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329_gshared)(__this, method);
}
// System.Void System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>::.ctor()
inline void List_1__ctor_m3339F822AD7AB85E44A3CE0FD82074C27CBB7903 (List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* __this, const RuntimeMethod* method)
{
	((  void (*) (List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5*, const RuntimeMethod*))List_1__ctor_m7F078BB342729BDF11327FD89D7872265328F690_gshared)(__this, method);
}
// System.Void Grpc.Core.ServerServiceDefinition::.ctor(System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ServerServiceDefinition__ctor_mF4BC7D215A7F6AECBA8865439697D6F8E2F9E405 (ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0* __this, List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* ___0_addMethodActions, const RuntimeMethod* method) ;
// System.Void Grpc.Core.SslCredentials::.ctor(System.String,Grpc.Core.KeyCertificatePair,Grpc.Core.VerifyPeerCallback)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SslCredentials__ctor_mA926FF2EF6211030E5971D99E987190B2DEB2E33 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, String_t* ___0_rootCertificates, KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* ___1_keyCertificatePair, VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* ___2_verifyPeerCallback, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Status::.ctor(Grpc.Core.StatusCode,System.String,System.Exception)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Status__ctor_m212E9F49C1AE80241D1C447DC18E3C09E3641D4E (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, int32_t ___0_statusCode, String_t* ___1_detail, Exception_t* ___2_debugException, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Status::.ctor(Grpc.Core.StatusCode,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Status__ctor_mC9E29F2BD60C9C3C1A3AD4A834052ABD9CBC89FF (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, int32_t ___0_statusCode, String_t* ___1_detail, const RuntimeMethod* method) ;
// System.String Grpc.Core.Status::get_Detail()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR String_t* Status_get_Detail_m27B09DC9C3459A6DE0461139E6F7012EA86C49BB_inline (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) ;
// System.Exception Grpc.Core.Status::get_DebugException()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Exception_t* Status_get_DebugException_m434DF404A6BDDD4910C19272ED5516F95500A359_inline (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) ;
// System.String System.String::Format(System.String,System.Object,System.Object,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* String_Format_mA0534D6E2AE4D67A6BD8D45B3321323930EB930C (String_t* ___0_format, RuntimeObject* ___1_arg0, RuntimeObject* ___2_arg1, RuntimeObject* ___3_arg2, const RuntimeMethod* method) ;
// System.Void Grpc.Core.WriteOptions::.ctor(Grpc.Core.WriteFlags)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void WriteOptions__ctor_m189F6B667BC45FC77CF649CC21B794DCC3F83FF5 (WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* __this, int32_t ___0_flags, const RuntimeMethod* method) ;
// System.Void System.ArgumentException::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ArgumentException__ctor_m34A925BA55EC4CE4253404E363B5F6A53EB51CA3 (ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263* __this, const RuntimeMethod* method) ;
// System.Void System.InvalidOperationException::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void InvalidOperationException__ctor_m1BE9BD198B904AA1D94F4B10DA88077DFD44B7A5 (InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB* __this, const RuntimeMethod* method) ;
// System.Void System.InvalidOperationException::.ctor(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void InvalidOperationException__ctor_mE4CB6F4712AB6D99A2358FBAE2E052B3EE976162 (InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB* __this, String_t* ___0_message, const RuntimeMethod* method) ;
// System.String System.String::CreateString(System.Char,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* String_CreateString_mAA0705B41B390BDB42F67894B9B67C956814C71B (String_t* __this, Il2CppChar ___0_c, int32_t ___1_count, const RuntimeMethod* method) ;
// System.Int32 System.Runtime.CompilerServices.RuntimeHelpers::get_OffsetToStringData()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t RuntimeHelpers_get_OffsetToStringData_m90A5D27EF88BE9432BF7093B7D7E7A0ACB0A8FBD (const RuntimeMethod* method) ;
// System.Void* System.IntPtr::ToPointer()
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void* IntPtr_ToPointer_m1A0612EED3A1C8B8850BE2943CFC42523064B4F6_inline (intptr_t* __this, const RuntimeMethod* method) ;
// System.String Grpc.Core.Api.Utils.EncodingExtensions::GetString(System.Text.Encoding,System.Byte*,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* EncodingExtensions_GetString_mF5E63092112C64E8D48B96690474700EBAFFD672 (Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___0_encoding, uint8_t* ___1_source, int32_t ___2_byteCount, const RuntimeMethod* method) ;
// System.Void Grpc.Core.Interceptors.InterceptingCallInvoker::.ctor(Grpc.Core.CallInvoker,Grpc.Core.Interceptors.Interceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void InterceptingCallInvoker__ctor_m32BAFD6EB2940991FD48CB27B3FAEFD82CF63C01 (InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4* __this, CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* ___1_interceptor, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.CallInvoker>(T,System.String)
inline CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* (*) (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.Interceptors.Interceptor[]>(T,System.String)
inline InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* GrpcPreconditions_CheckNotNull_TisInterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2_m54DCBCEF26FDD11320936B2E59AE28F2E7CAC4CC (InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* (*) (InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// System.Collections.Generic.IEnumerable`1<TSource> System.Linq.Enumerable::Reverse<Grpc.Core.Interceptors.Interceptor>(System.Collections.Generic.IEnumerable`1<TSource>)
inline RuntimeObject* Enumerable_Reverse_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m1C1BF18516E49D40D7DBDB84100A0B8168E2B4BA (RuntimeObject* ___0_source, const RuntimeMethod* method)
{
	return ((  RuntimeObject* (*) (RuntimeObject*, const RuntimeMethod*))Enumerable_Reverse_TisRuntimeObject_mF7B0A079765A37716D52A158EBD61E57C390CBC9_gshared)(___0_source, method);
}
// System.Void Grpc.Core.Interceptors.CallInvokerExtensions/MetadataInterceptor::.ctor(System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void MetadataInterceptor__ctor_mE4EAD6608BAD8917BB3873CF2245A67A2F9DEAA3 (MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975* __this, Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* ___0_interceptor, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>>(T,System.String)
inline Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* GrpcPreconditions_CheckNotNull_TisFunc_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B_m5E04E5E300A4C3AD62F603401B642AB7C7656305 (Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* (*) (Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.CallInvokerExtensions::Intercept(Grpc.Core.CallInvoker,Grpc.Core.Interceptors.Interceptor[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* CallInvokerExtensions_Intercept_m5D4276B12009D07A5A34D25827644546264A071B (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* ___1_interceptors, const RuntimeMethod* method) ;
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.CallInvokerExtensions::Intercept(Grpc.Core.CallInvoker,System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* CallInvokerExtensions_Intercept_mE077F5FBF93ED074216668E3CD469F4F260C300F (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* ___1_interceptor, const RuntimeMethod* method) ;
// System.Void Grpc.Core.CallInvoker::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CallInvoker__ctor_mFFDA4B00606F063F1EAC9DBBCDDECD9F82705E6B (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* __this, const RuntimeMethod* method) ;
// T Grpc.Core.Utils.GrpcPreconditions::CheckNotNull<Grpc.Core.Interceptors.Interceptor>(T,System.String)
inline Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* GrpcPreconditions_CheckNotNull_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m4E082761E5C0C7589B1EA55BB46E22E68FDA6B54 (Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* ___0_reference, String_t* ___1_paramName, const RuntimeMethod* method)
{
	return ((  Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* (*) (Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0*, String_t*, const RuntimeMethod*))GrpcPreconditions_CheckNotNull_TisRuntimeObject_m8A5A013EDA379D3D466EFFDBAC1C82844EAE43A5_gshared)(___0_reference, ___1_paramName, method);
}
// System.Void System.Array::Clear(System.Array,System.Int32,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Array_Clear_m50BAA3751899858B097D3FF2ED31F284703FE5CB (RuntimeArray* ___0_array, int32_t ___1_index, int32_t ___2_length, const RuntimeMethod* method) ;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Microsoft.CodeAnalysis.EmbeddedAttribute::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void EmbeddedAttribute__ctor_mC3858E56C6A4D19D77D08807E2E4390E4F471AE0 (EmbeddedAttribute_t1EE07D5A21E2F81B6A651A3843A9BE8E5ADEE16D* __this, const RuntimeMethod* method) 
{
	{
		Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void System.Runtime.CompilerServices.IsReadOnlyAttribute::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void IsReadOnlyAttribute__ctor_m36A6EAE24408BF0FB8ECEEAFE6E7B5B469C105AE (IsReadOnlyAttribute_t0A4E747F0A0347653B3EC39C15485282851A10B2* __this, const RuntimeMethod* method) 
{
	{
		Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void System.Runtime.CompilerServices.NullableAttribute::.ctor(System.Byte)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void NullableAttribute__ctor_mC24EE06E1EDFA2881827E536E92FA832AAA2946E (NullableAttribute_t7181E30ED50BDD358745EA005FCEC8C87AEF5EBA* __this, uint8_t ___0_p, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2(__this, NULL);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)SZArrayNew(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var, (uint32_t)1);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_1 = L_0;
		uint8_t L_2 = ___0_p;
		NullCheck(L_1);
		(L_1)->SetAt(static_cast<il2cpp_array_size_t>(0), (uint8_t)L_2);
		__this->___NullableFlags_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___NullableFlags_0), (void*)L_1);
		return;
	}
}
// System.Void System.Runtime.CompilerServices.NullableAttribute::.ctor(System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void NullableAttribute__ctor_m3F10678531D743811AE9F964BF6621DDF34BBB88 (NullableAttribute_t7181E30ED50BDD358745EA005FCEC8C87AEF5EBA* __this, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___0_p, const RuntimeMethod* method) 
{
	{
		Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2(__this, NULL);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = ___0_p;
		__this->___NullableFlags_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___NullableFlags_0), (void*)L_0);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void System.Runtime.CompilerServices.NullableContextAttribute::.ctor(System.Byte)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void NullableContextAttribute__ctor_mA3048C37A0BD29A5560584EFFFDF71AE9C142448 (NullableContextAttribute_t4A7A18FD00A42573002F14FA54763772B783B64C* __this, uint8_t ___0_p, const RuntimeMethod* method) 
{
	{
		Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2(__this, NULL);
		uint8_t L_0 = ___0_p;
		__this->___Flag_0 = L_0;
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void System.Diagnostics.CodeAnalysis.DynamicallyAccessedMembersAttribute::.ctor(System.Diagnostics.CodeAnalysis.DynamicallyAccessedMemberTypes)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void DynamicallyAccessedMembersAttribute__ctor_mADF99ECAC5E81C3938BC81BC9F5F37561B75FBFC (DynamicallyAccessedMembersAttribute_t2F26B97D513CC0C186D7433DD2935BDED77C481E* __this, int32_t ___0_memberTypes, const RuntimeMethod* method) 
{
	{
		Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2(__this, NULL);
		int32_t L_0 = ___0_memberTypes;
		__this->___U3CMemberTypesU3Ek__BackingField_0 = L_0;
		return;
	}
}
// System.Diagnostics.CodeAnalysis.DynamicallyAccessedMemberTypes System.Diagnostics.CodeAnalysis.DynamicallyAccessedMembersAttribute::get_MemberTypes()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t DynamicallyAccessedMembersAttribute_get_MemberTypes_m633395EEC2A03B0F83DC11053970F9504FBCD36E (DynamicallyAccessedMembersAttribute_t2F26B97D513CC0C186D7433DD2935BDED77C481E* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->___U3CMemberTypesU3Ek__BackingField_0;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_Multicast(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	il2cpp_array_size_t length = __this->___delegates_13->max_length;
	Delegate_t** delegatesToInvoke = reinterpret_cast<Delegate_t**>(__this->___delegates_13->GetAddressAtUnchecked(0));
	Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* retVal = NULL;
	for (il2cpp_array_size_t i = 0; i < length; i++)
	{
		AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* currentDelegate = reinterpret_cast<AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968*>(delegatesToInvoke[i]);
		typedef Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* (*FunctionPointerType) (RuntimeObject*, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06*, const RuntimeMethod*);
		retVal = ((FunctionPointerType)currentDelegate->___invoke_impl_1)((Il2CppObject*)currentDelegate->___method_code_6, ___0_context, ___1_metadata, reinterpret_cast<RuntimeMethod*>(currentDelegate->___method_3));
	}
	return retVal;
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenInst(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	typedef Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* (*FunctionPointerType) (AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___method_ptr_0)(___0_context, ___1_metadata, method);
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenStatic(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	typedef Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* (*FunctionPointerType) (AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___method_ptr_0)(___0_context, ___1_metadata, method);
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenStaticInvoker(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	return InvokerFuncInvoker2< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(__this->___method_ptr_0, method, NULL, ___0_context, ___1_metadata);
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_ClosedStaticInvoker(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	return InvokerFuncInvoker3< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*, RuntimeObject*, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(__this->___method_ptr_0, method, NULL, __this->___m_target_2, ___0_context, ___1_metadata);
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenVirtual(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return VirtualFuncInvoker1< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(il2cpp_codegen_method_get_slot(method), ___0_context, ___1_metadata);
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenInterface(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return InterfaceFuncInvoker1< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(il2cpp_codegen_method_get_slot(method), il2cpp_codegen_method_get_declaring_type(method), ___0_context, ___1_metadata);
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenGenericVirtual(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return GenericVirtualFuncInvoker1< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(method, ___0_context, ___1_metadata);
}
Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenGenericInterface(AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return GenericInterfaceFuncInvoker1< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(method, ___0_context, ___1_metadata);
}
// System.Void Grpc.Core.AsyncAuthInterceptor::.ctor(System.Object,System.IntPtr)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncAuthInterceptor__ctor_mDB48D8B92FC72C188D559E2E18A8DFB0D414E960 (AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method) 
{
	__this->___method_ptr_0 = il2cpp_codegen_get_virtual_call_method_pointer((RuntimeMethod*)___1_method);
	__this->___method_3 = ___1_method;
	__this->___m_target_2 = ___0_object;
	Il2CppCodeGenWriteBarrier((void**)(&__this->___m_target_2), (void*)___0_object);
	int parameterCount = il2cpp_codegen_method_parameter_count((RuntimeMethod*)___1_method);
	__this->___method_code_6 = (intptr_t)__this;
	if (MethodIsStatic((RuntimeMethod*)___1_method))
	{
		bool isOpen = parameterCount == 2;
		if (il2cpp_codegen_call_method_via_invoker((RuntimeMethod*)___1_method))
			if (isOpen)
				__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenStaticInvoker;
			else
				__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_ClosedStaticInvoker;
		else
			if (isOpen)
				__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenStatic;
			else
				{
					__this->___invoke_impl_1 = (intptr_t)__this->___method_ptr_0;
					__this->___method_code_6 = (intptr_t)__this->___m_target_2;
				}
	}
	else
	{
		bool isOpen = parameterCount == 1;
		if (isOpen)
		{
			if (__this->___method_is_virtual_12)
			{
				if (il2cpp_codegen_method_is_generic_instance_method((RuntimeMethod*)___1_method))
					if (il2cpp_codegen_method_is_interface_method((RuntimeMethod*)___1_method))
						__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenGenericInterface;
					else
						__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenGenericVirtual;
				else
					if (il2cpp_codegen_method_is_interface_method((RuntimeMethod*)___1_method))
						__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenInterface;
					else
						__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenVirtual;
			}
			else
			{
				__this->___invoke_impl_1 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_OpenInst;
			}
		}
		else
		{
			if (___0_object == NULL)
				il2cpp_codegen_raise_exception(il2cpp_codegen_get_argument_exception(NULL, "Delegate to an instance method cannot have null 'this'."), NULL);
			__this->___invoke_impl_1 = (intptr_t)__this->___method_ptr_0;
			__this->___method_code_6 = (intptr_t)__this->___m_target_2;
		}
	}
	__this->___extra_arg_5 = (intptr_t)&AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0_Multicast;
}
// System.Threading.Tasks.Task Grpc.Core.AsyncAuthInterceptor::Invoke(Grpc.Core.AuthInterceptorContext,Grpc.Core.Metadata)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_Invoke_mF9FDA0CC8A5D0B1404E9040550C9B28D0161DDB0 (AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, const RuntimeMethod* method) 
{
	typedef Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* (*FunctionPointerType) (RuntimeObject*, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, ___0_context, ___1_metadata, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
// System.IAsyncResult Grpc.Core.AsyncAuthInterceptor::BeginInvoke(Grpc.Core.AuthInterceptorContext,Grpc.Core.Metadata,System.AsyncCallback,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* AsyncAuthInterceptor_BeginInvoke_m5EB6CA9AD1558C07ECCA2909576F6E9B673CA5C5 (AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* ___0_context, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_metadata, AsyncCallback_t7FEF460CBDCFB9C5FA2EF776984778B9A4145F4C* ___2_callback, RuntimeObject* ___3_object, const RuntimeMethod* method) 
{
	void *__d_args[3] = {0};
	__d_args[0] = ___0_context;
	__d_args[1] = ___1_metadata;
	return (RuntimeObject*)il2cpp_codegen_delegate_begin_invoke((RuntimeDelegate*)__this, __d_args, (RuntimeDelegate*)___2_callback, (RuntimeObject*)___3_object);
}
// System.Threading.Tasks.Task Grpc.Core.AsyncAuthInterceptor::EndInvoke(System.IAsyncResult)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* AsyncAuthInterceptor_EndInvoke_mFC414DE8CB0A063970D731C9E746C95421CE2429 (AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* __this, RuntimeObject* ___0_result, const RuntimeMethod* method) 
{
	RuntimeObject *__result = il2cpp_codegen_delegate_end_invoke((Il2CppAsyncResult*) ___0_result, 0);
	return (Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*)__result;
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.AuthInterceptorContext::.ctor(System.String,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AuthInterceptorContext__ctor_m05ECF85A575306F8BA979B3F171F24BB5A63EA66 (AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* __this, String_t* ___0_serviceUrl, String_t* ___1_methodName, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_serviceUrl;
		String_t* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9(L_0, GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9_RuntimeMethod_var);
		__this->___serviceUrl_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___serviceUrl_0), (void*)L_1);
		String_t* L_2 = ___1_methodName;
		String_t* L_3;
		L_3 = GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9(L_2, GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9_RuntimeMethod_var);
		__this->___methodName_1 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___methodName_1), (void*)L_3);
		return;
	}
}
// System.String Grpc.Core.AuthInterceptorContext::get_ServiceUrl()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* AuthInterceptorContext_get_ServiceUrl_m504DD9957CF7BF4F7F40922E63CEC5997E450008 (AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___serviceUrl_0;
		return L_0;
	}
}
// System.String Grpc.Core.AuthInterceptorContext::get_MethodName()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* AuthInterceptorContext_get_MethodName_m517667030F7579FD672FA390F7CA158B4D5117B6 (AuthInterceptorContext_t9EA08A01E0CC43951F43A2E8BC19B619AA49E52D* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___methodName_1;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// Conversion methods for marshalling of: Grpc.Core.AsyncCallState
IL2CPP_EXTERN_C void AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshal_pinvoke(const AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920& unmarshaled, AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_pinvoke& marshaled)
{
	if (unmarshaled.___responseHeadersAsync_0 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___responseHeadersAsync_0))
		{
			marshaled.___responseHeadersAsync_0 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___responseHeadersAsync_0));
			(marshaled.___responseHeadersAsync_0)->AddRef();
		}
		else
		{
			marshaled.___responseHeadersAsync_0 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___responseHeadersAsync_0);
		}
	}
	else
	{
		marshaled.___responseHeadersAsync_0 = NULL;
	}
	if (unmarshaled.___getStatusFunc_1 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getStatusFunc_1))
		{
			marshaled.___getStatusFunc_1 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___getStatusFunc_1));
			(marshaled.___getStatusFunc_1)->AddRef();
		}
		else
		{
			marshaled.___getStatusFunc_1 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___getStatusFunc_1);
		}
	}
	else
	{
		marshaled.___getStatusFunc_1 = NULL;
	}
	if (unmarshaled.___getTrailersFunc_2 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getTrailersFunc_2))
		{
			marshaled.___getTrailersFunc_2 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___getTrailersFunc_2));
			(marshaled.___getTrailersFunc_2)->AddRef();
		}
		else
		{
			marshaled.___getTrailersFunc_2 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___getTrailersFunc_2);
		}
	}
	else
	{
		marshaled.___getTrailersFunc_2 = NULL;
	}
	if (unmarshaled.___disposeAction_3 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___disposeAction_3))
		{
			marshaled.___disposeAction_3 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___disposeAction_3));
			(marshaled.___disposeAction_3)->AddRef();
		}
		else
		{
			marshaled.___disposeAction_3 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___disposeAction_3);
		}
	}
	else
	{
		marshaled.___disposeAction_3 = NULL;
	}
	if (unmarshaled.___callbackState_4 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___callbackState_4))
		{
			marshaled.___callbackState_4 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___callbackState_4));
			(marshaled.___callbackState_4)->AddRef();
		}
		else
		{
			marshaled.___callbackState_4 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___callbackState_4);
		}
	}
	else
	{
		marshaled.___callbackState_4 = NULL;
	}
}
IL2CPP_EXTERN_C void AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshal_pinvoke_back(const AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_pinvoke& marshaled, AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920& unmarshaled)
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Il2CppComObject_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	if (marshaled.___responseHeadersAsync_0 != NULL)
	{
		unmarshaled.___responseHeadersAsync_0 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___responseHeadersAsync_0, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___responseHeadersAsync_0), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___responseHeadersAsync_0, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___responseHeadersAsync_0))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___responseHeadersAsync_0), Il2CppIUnknown::IID, marshaled.___responseHeadersAsync_0);
		}
	}
	else
	{
		unmarshaled.___responseHeadersAsync_0 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___responseHeadersAsync_0), (void*)NULL);
	}
	if (marshaled.___getStatusFunc_1 != NULL)
	{
		unmarshaled.___getStatusFunc_1 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getStatusFunc_1, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getStatusFunc_1), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getStatusFunc_1, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getStatusFunc_1))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___getStatusFunc_1), Il2CppIUnknown::IID, marshaled.___getStatusFunc_1);
		}
	}
	else
	{
		unmarshaled.___getStatusFunc_1 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getStatusFunc_1), (void*)NULL);
	}
	if (marshaled.___getTrailersFunc_2 != NULL)
	{
		unmarshaled.___getTrailersFunc_2 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getTrailersFunc_2, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getTrailersFunc_2), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getTrailersFunc_2, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getTrailersFunc_2))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___getTrailersFunc_2), Il2CppIUnknown::IID, marshaled.___getTrailersFunc_2);
		}
	}
	else
	{
		unmarshaled.___getTrailersFunc_2 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getTrailersFunc_2), (void*)NULL);
	}
	if (marshaled.___disposeAction_3 != NULL)
	{
		unmarshaled.___disposeAction_3 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___disposeAction_3, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___disposeAction_3), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___disposeAction_3, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___disposeAction_3))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___disposeAction_3), Il2CppIUnknown::IID, marshaled.___disposeAction_3);
		}
	}
	else
	{
		unmarshaled.___disposeAction_3 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___disposeAction_3), (void*)NULL);
	}
	if (marshaled.___callbackState_4 != NULL)
	{
		unmarshaled.___callbackState_4 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___callbackState_4, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___callbackState_4), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___callbackState_4, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___callbackState_4))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___callbackState_4), Il2CppIUnknown::IID, marshaled.___callbackState_4);
		}
	}
	else
	{
		unmarshaled.___callbackState_4 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___callbackState_4), (void*)NULL);
	}
}
// Conversion method for clean up from marshalling of: Grpc.Core.AsyncCallState
IL2CPP_EXTERN_C void AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshal_pinvoke_cleanup(AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_pinvoke& marshaled)
{
	if (marshaled.___responseHeadersAsync_0 != NULL)
	{
		(marshaled.___responseHeadersAsync_0)->Release();
		marshaled.___responseHeadersAsync_0 = NULL;
	}
	if (marshaled.___getStatusFunc_1 != NULL)
	{
		(marshaled.___getStatusFunc_1)->Release();
		marshaled.___getStatusFunc_1 = NULL;
	}
	if (marshaled.___getTrailersFunc_2 != NULL)
	{
		(marshaled.___getTrailersFunc_2)->Release();
		marshaled.___getTrailersFunc_2 = NULL;
	}
	if (marshaled.___disposeAction_3 != NULL)
	{
		(marshaled.___disposeAction_3)->Release();
		marshaled.___disposeAction_3 = NULL;
	}
	if (marshaled.___callbackState_4 != NULL)
	{
		(marshaled.___callbackState_4)->Release();
		marshaled.___callbackState_4 = NULL;
	}
}
// Conversion methods for marshalling of: Grpc.Core.AsyncCallState
IL2CPP_EXTERN_C void AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshal_com(const AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920& unmarshaled, AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_com& marshaled)
{
	if (unmarshaled.___responseHeadersAsync_0 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___responseHeadersAsync_0))
		{
			marshaled.___responseHeadersAsync_0 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___responseHeadersAsync_0));
			(marshaled.___responseHeadersAsync_0)->AddRef();
		}
		else
		{
			marshaled.___responseHeadersAsync_0 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___responseHeadersAsync_0);
		}
	}
	else
	{
		marshaled.___responseHeadersAsync_0 = NULL;
	}
	if (unmarshaled.___getStatusFunc_1 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getStatusFunc_1))
		{
			marshaled.___getStatusFunc_1 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___getStatusFunc_1));
			(marshaled.___getStatusFunc_1)->AddRef();
		}
		else
		{
			marshaled.___getStatusFunc_1 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___getStatusFunc_1);
		}
	}
	else
	{
		marshaled.___getStatusFunc_1 = NULL;
	}
	if (unmarshaled.___getTrailersFunc_2 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getTrailersFunc_2))
		{
			marshaled.___getTrailersFunc_2 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___getTrailersFunc_2));
			(marshaled.___getTrailersFunc_2)->AddRef();
		}
		else
		{
			marshaled.___getTrailersFunc_2 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___getTrailersFunc_2);
		}
	}
	else
	{
		marshaled.___getTrailersFunc_2 = NULL;
	}
	if (unmarshaled.___disposeAction_3 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___disposeAction_3))
		{
			marshaled.___disposeAction_3 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___disposeAction_3));
			(marshaled.___disposeAction_3)->AddRef();
		}
		else
		{
			marshaled.___disposeAction_3 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___disposeAction_3);
		}
	}
	else
	{
		marshaled.___disposeAction_3 = NULL;
	}
	if (unmarshaled.___callbackState_4 != NULL)
	{
		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___callbackState_4))
		{
			marshaled.___callbackState_4 = il2cpp_codegen_com_query_interface<Il2CppIUnknown>(static_cast<Il2CppComObject*>(unmarshaled.___callbackState_4));
			(marshaled.___callbackState_4)->AddRef();
		}
		else
		{
			marshaled.___callbackState_4 = il2cpp_codegen_com_get_or_create_ccw<Il2CppIUnknown>(unmarshaled.___callbackState_4);
		}
	}
	else
	{
		marshaled.___callbackState_4 = NULL;
	}
}
IL2CPP_EXTERN_C void AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshal_com_back(const AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_com& marshaled, AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920& unmarshaled)
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Il2CppComObject_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	if (marshaled.___responseHeadersAsync_0 != NULL)
	{
		unmarshaled.___responseHeadersAsync_0 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___responseHeadersAsync_0, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___responseHeadersAsync_0), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___responseHeadersAsync_0, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___responseHeadersAsync_0))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___responseHeadersAsync_0), Il2CppIUnknown::IID, marshaled.___responseHeadersAsync_0);
		}
	}
	else
	{
		unmarshaled.___responseHeadersAsync_0 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___responseHeadersAsync_0), (void*)NULL);
	}
	if (marshaled.___getStatusFunc_1 != NULL)
	{
		unmarshaled.___getStatusFunc_1 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getStatusFunc_1, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getStatusFunc_1), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getStatusFunc_1, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getStatusFunc_1))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___getStatusFunc_1), Il2CppIUnknown::IID, marshaled.___getStatusFunc_1);
		}
	}
	else
	{
		unmarshaled.___getStatusFunc_1 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getStatusFunc_1), (void*)NULL);
	}
	if (marshaled.___getTrailersFunc_2 != NULL)
	{
		unmarshaled.___getTrailersFunc_2 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getTrailersFunc_2, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getTrailersFunc_2), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___getTrailersFunc_2, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___getTrailersFunc_2))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___getTrailersFunc_2), Il2CppIUnknown::IID, marshaled.___getTrailersFunc_2);
		}
	}
	else
	{
		unmarshaled.___getTrailersFunc_2 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___getTrailersFunc_2), (void*)NULL);
	}
	if (marshaled.___disposeAction_3 != NULL)
	{
		unmarshaled.___disposeAction_3 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___disposeAction_3, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___disposeAction_3), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___disposeAction_3, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___disposeAction_3))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___disposeAction_3), Il2CppIUnknown::IID, marshaled.___disposeAction_3);
		}
	}
	else
	{
		unmarshaled.___disposeAction_3 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___disposeAction_3), (void*)NULL);
	}
	if (marshaled.___callbackState_4 != NULL)
	{
		unmarshaled.___callbackState_4 = il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___callbackState_4, Il2CppComObject_il2cpp_TypeInfo_var);
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___callbackState_4), (void*)il2cpp_codegen_com_get_or_create_rcw_from_iunknown<RuntimeObject>(marshaled.___callbackState_4, Il2CppComObject_il2cpp_TypeInfo_var));

		if (il2cpp_codegen_is_import_or_windows_runtime(unmarshaled.___callbackState_4))
		{
			il2cpp_codegen_com_cache_queried_interface(static_cast<Il2CppComObject*>(unmarshaled.___callbackState_4), Il2CppIUnknown::IID, marshaled.___callbackState_4);
		}
	}
	else
	{
		unmarshaled.___callbackState_4 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&unmarshaled.___callbackState_4), (void*)NULL);
	}
}
// Conversion method for clean up from marshalling of: Grpc.Core.AsyncCallState
IL2CPP_EXTERN_C void AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshal_com_cleanup(AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920_marshaled_com& marshaled)
{
	if (marshaled.___responseHeadersAsync_0 != NULL)
	{
		(marshaled.___responseHeadersAsync_0)->Release();
		marshaled.___responseHeadersAsync_0 = NULL;
	}
	if (marshaled.___getStatusFunc_1 != NULL)
	{
		(marshaled.___getStatusFunc_1)->Release();
		marshaled.___getStatusFunc_1 = NULL;
	}
	if (marshaled.___getTrailersFunc_2 != NULL)
	{
		(marshaled.___getTrailersFunc_2)->Release();
		marshaled.___getTrailersFunc_2 = NULL;
	}
	if (marshaled.___disposeAction_3 != NULL)
	{
		(marshaled.___disposeAction_3)->Release();
		marshaled.___disposeAction_3 = NULL;
	}
	if (marshaled.___callbackState_4 != NULL)
	{
		(marshaled.___callbackState_4)->Release();
		marshaled.___callbackState_4 = NULL;
	}
}
// System.Void Grpc.Core.AsyncCallState::.ctor(System.Func`2<System.Object,System.Threading.Tasks.Task`1<Grpc.Core.Metadata>>,System.Func`2<System.Object,Grpc.Core.Status>,System.Func`2<System.Object,Grpc.Core.Metadata>,System.Action`1<System.Object>,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncCallState__ctor_m2761326419B34F462224F8BD6981EA747039F704 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* ___0_responseHeadersAsync, Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* ___1_getStatusFunc, Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* ___2_getTrailersFunc, Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* ___3_disposeAction, RuntimeObject* ___4_callbackState, const RuntimeMethod* method) 
{
	{
		Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* L_0 = ___0_responseHeadersAsync;
		__this->___responseHeadersAsync_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___responseHeadersAsync_0), (void*)L_0);
		Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* L_1 = ___1_getStatusFunc;
		__this->___getStatusFunc_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___getStatusFunc_1), (void*)L_1);
		Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* L_2 = ___2_getTrailersFunc;
		__this->___getTrailersFunc_2 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___getTrailersFunc_2), (void*)L_2);
		Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* L_3 = ___3_disposeAction;
		__this->___disposeAction_3 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___disposeAction_3), (void*)L_3);
		RuntimeObject* L_4 = ___4_callbackState;
		__this->___callbackState_4 = L_4;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___callbackState_4), (void*)L_4);
		return;
	}
}
IL2CPP_EXTERN_C  void AsyncCallState__ctor_m2761326419B34F462224F8BD6981EA747039F704_AdjustorThunk (RuntimeObject* __this, Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* ___0_responseHeadersAsync, Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* ___1_getStatusFunc, Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* ___2_getTrailersFunc, Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* ___3_disposeAction, RuntimeObject* ___4_callbackState, const RuntimeMethod* method)
{
	AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920*>(__this + _offset);
	AsyncCallState__ctor_m2761326419B34F462224F8BD6981EA747039F704(_thisAdjusted, ___0_responseHeadersAsync, ___1_getStatusFunc, ___2_getTrailersFunc, ___3_disposeAction, ___4_callbackState, method);
}
// System.Void Grpc.Core.AsyncCallState::.ctor(System.Threading.Tasks.Task`1<Grpc.Core.Metadata>,System.Func`1<Grpc.Core.Status>,System.Func`1<Grpc.Core.Metadata>,System.Action)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncCallState__ctor_m257BB7A5A2835AD2A035D5ED6A325A2CAA3B46F0 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* ___0_responseHeadersAsync, Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0* ___1_getStatusFunc, Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C* ___2_getTrailersFunc, Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07* ___3_disposeAction, const RuntimeMethod* method) 
{
	{
		Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* L_0 = ___0_responseHeadersAsync;
		__this->___responseHeadersAsync_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___responseHeadersAsync_0), (void*)L_0);
		Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0* L_1 = ___1_getStatusFunc;
		__this->___getStatusFunc_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___getStatusFunc_1), (void*)L_1);
		Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C* L_2 = ___2_getTrailersFunc;
		__this->___getTrailersFunc_2 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___getTrailersFunc_2), (void*)L_2);
		Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07* L_3 = ___3_disposeAction;
		__this->___disposeAction_3 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___disposeAction_3), (void*)L_3);
		__this->___callbackState_4 = NULL;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___callbackState_4), (void*)NULL);
		return;
	}
}
IL2CPP_EXTERN_C  void AsyncCallState__ctor_m257BB7A5A2835AD2A035D5ED6A325A2CAA3B46F0_AdjustorThunk (RuntimeObject* __this, Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* ___0_responseHeadersAsync, Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0* ___1_getStatusFunc, Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C* ___2_getTrailersFunc, Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07* ___3_disposeAction, const RuntimeMethod* method)
{
	AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920*>(__this + _offset);
	AsyncCallState__ctor_m257BB7A5A2835AD2A035D5ED6A325A2CAA3B46F0(_thisAdjusted, ___0_responseHeadersAsync, ___1_getStatusFunc, ___2_getTrailersFunc, ___3_disposeAction, method);
}
// System.Threading.Tasks.Task`1<Grpc.Core.Metadata> Grpc.Core.AsyncCallState::ResponseHeadersAsync()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* AsyncCallState_ResponseHeadersAsync_m700324CB96BA18E2DB0FFDA1FC83C048EA03CEA5 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* V_0 = NULL;
	{
		RuntimeObject* L_0 = __this->___responseHeadersAsync_0;
		V_0 = ((Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974*)IsInst((RuntimeObject*)L_0, Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974_il2cpp_TypeInfo_var));
		Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* L_1 = V_0;
		if (L_1)
		{
			goto IL_001b;
		}
	}
	{
		RuntimeObject* L_2 = __this->___responseHeadersAsync_0;
		return ((Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707*)CastclassClass((RuntimeObject*)L_2, Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707_il2cpp_TypeInfo_var));
	}

IL_001b:
	{
		Func_2_t63ECDFD91FC7A17D2304051A106F27976E05A974* L_3 = V_0;
		RuntimeObject* L_4 = __this->___callbackState_4;
		NullCheck(L_3);
		Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* L_5;
		L_5 = Func_2_Invoke_m5B8E3DC21ABC9447271E3B648193324267F53679_inline(L_3, L_4, NULL);
		return L_5;
	}
}
IL2CPP_EXTERN_C  Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* AsyncCallState_ResponseHeadersAsync_m700324CB96BA18E2DB0FFDA1FC83C048EA03CEA5_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920*>(__this + _offset);
	Task_1_tC360F8DCFEAB695B7D285BA2DA419A6C8ECC0707* _returnValue;
	_returnValue = AsyncCallState_ResponseHeadersAsync_m700324CB96BA18E2DB0FFDA1FC83C048EA03CEA5(_thisAdjusted, method);
	return _returnValue;
}
// Grpc.Core.Status Grpc.Core.AsyncCallState::GetStatus()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 AsyncCallState_GetStatus_mB33352422BEB5CEE4C03E69A3E68C1EC01B8D265 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* V_0 = NULL;
	{
		RuntimeObject* L_0 = __this->___getStatusFunc_1;
		V_0 = ((Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2*)IsInst((RuntimeObject*)L_0, Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2_il2cpp_TypeInfo_var));
		Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* L_1 = V_0;
		if (L_1)
		{
			goto IL_0020;
		}
	}
	{
		RuntimeObject* L_2 = __this->___getStatusFunc_1;
		NullCheck(((Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0*)Castclass((RuntimeObject*)L_2, Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0_il2cpp_TypeInfo_var)));
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_3;
		L_3 = Func_1_Invoke_m349A2DF2DF3672A6D87AA4E5E86E8AF54B24A956_inline(((Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0*)Castclass((RuntimeObject*)L_2, Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0_il2cpp_TypeInfo_var)), NULL);
		return L_3;
	}

IL_0020:
	{
		Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* L_4 = V_0;
		RuntimeObject* L_5 = __this->___callbackState_4;
		NullCheck(L_4);
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_6;
		L_6 = Func_2_Invoke_m497B8CC89F1330EB7265E70F432347348B580F14_inline(L_4, L_5, NULL);
		return L_6;
	}
}
IL2CPP_EXTERN_C  Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 AsyncCallState_GetStatus_mB33352422BEB5CEE4C03E69A3E68C1EC01B8D265_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920*>(__this + _offset);
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 _returnValue;
	_returnValue = AsyncCallState_GetStatus_mB33352422BEB5CEE4C03E69A3E68C1EC01B8D265(_thisAdjusted, method);
	return _returnValue;
}
// Grpc.Core.Metadata Grpc.Core.AsyncCallState::GetTrailers()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* AsyncCallState_GetTrailers_m5E344A799E41C38135EBB3B5BC179E11BFAF46D5 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* V_0 = NULL;
	{
		RuntimeObject* L_0 = __this->___getTrailersFunc_2;
		V_0 = ((Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A*)IsInst((RuntimeObject*)L_0, Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A_il2cpp_TypeInfo_var));
		Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* L_1 = V_0;
		if (L_1)
		{
			goto IL_0020;
		}
	}
	{
		RuntimeObject* L_2 = __this->___getTrailersFunc_2;
		NullCheck(((Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C*)Castclass((RuntimeObject*)L_2, Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C_il2cpp_TypeInfo_var)));
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_3;
		L_3 = Func_1_Invoke_m3E136C280A87E956C5A2B8EC105691F5AAAB293D_inline(((Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C*)Castclass((RuntimeObject*)L_2, Func_1_t5597F0C90B2EAC4257FA7D8FF94EFCD14B6EAA5C_il2cpp_TypeInfo_var)), NULL);
		return L_3;
	}

IL_0020:
	{
		Func_2_t6B58D999A75F59C5A5A5E8735D5D295D06BAF60A* L_4 = V_0;
		RuntimeObject* L_5 = __this->___callbackState_4;
		NullCheck(L_4);
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_6;
		L_6 = Func_2_Invoke_m1EDA519D53C4FE8082F91071B2A383E9788FFA4B_inline(L_4, L_5, NULL);
		return L_6;
	}
}
IL2CPP_EXTERN_C  Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* AsyncCallState_GetTrailers_m5E344A799E41C38135EBB3B5BC179E11BFAF46D5_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920*>(__this + _offset);
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* _returnValue;
	_returnValue = AsyncCallState_GetTrailers_m5E344A799E41C38135EBB3B5BC179E11BFAF46D5(_thisAdjusted, method);
	return _returnValue;
}
// System.Void Grpc.Core.AsyncCallState::Dispose()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncCallState_Dispose_m92ACEB8E0BB514EE0DF548E9CF5F8903236E5A70 (AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* V_0 = NULL;
	{
		RuntimeObject* L_0 = __this->___disposeAction_3;
		V_0 = ((Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87*)IsInst((RuntimeObject*)L_0, Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87_il2cpp_TypeInfo_var));
		Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* L_1 = V_0;
		if (!L_1)
		{
			goto IL_001c;
		}
	}
	{
		Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* L_2 = V_0;
		RuntimeObject* L_3 = __this->___callbackState_4;
		NullCheck(L_2);
		Action_1_Invoke_mF2422B2DD29F74CE66F791C3F68E288EC7C3DB9E_inline(L_2, L_3, NULL);
		return;
	}

IL_001c:
	{
		RuntimeObject* L_4 = __this->___disposeAction_3;
		NullCheck(((Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07*)CastclassSealed((RuntimeObject*)L_4, Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07_il2cpp_TypeInfo_var)));
		Action_Invoke_m7126A54DACA72B845424072887B5F3A51FC3808E_inline(((Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07*)CastclassSealed((RuntimeObject*)L_4, Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07_il2cpp_TypeInfo_var)), NULL);
		return;
	}
}
IL2CPP_EXTERN_C  void AsyncCallState_Dispose_m92ACEB8E0BB514EE0DF548E9CF5F8903236E5A70_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<AsyncCallState_t4925877EB4B16440DA661E0661C766EB271C3920*>(__this + _offset);
	AsyncCallState_Dispose_m92ACEB8E0BB514EE0DF548E9CF5F8903236E5A70(_thisAdjusted, method);
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.AuthContext::.ctor(System.String,System.Collections.Generic.Dictionary`2<System.String,System.Collections.Generic.List`1<Grpc.Core.AuthProperty>>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AuthContext__ctor_m86C9F55BC62533F394B5942078A49593CC845B29 (AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* __this, String_t* ___0_peerIdentityPropertyName, Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* ___1_properties, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisDictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50_m7ED3349BDCE75D560CFB3D131E416BFB92D48222_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_peerIdentityPropertyName;
		__this->___peerIdentityPropertyName_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___peerIdentityPropertyName_0), (void*)L_0);
		Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* L_1 = ___1_properties;
		Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* L_2;
		L_2 = GrpcPreconditions_CheckNotNull_TisDictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50_m7ED3349BDCE75D560CFB3D131E416BFB92D48222(L_1, GrpcPreconditions_CheckNotNull_TisDictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50_m7ED3349BDCE75D560CFB3D131E416BFB92D48222_RuntimeMethod_var);
		__this->___properties_1 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___properties_1), (void*)L_2);
		return;
	}
}
// System.Boolean Grpc.Core.AuthContext::get_IsPeerAuthenticated()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool AuthContext_get_IsPeerAuthenticated_mFF7E42331F657835B788E41865E75796D7BC2FC9 (AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___peerIdentityPropertyName_0;
		return (bool)((!(((RuntimeObject*)(String_t*)L_0) <= ((RuntimeObject*)(RuntimeObject*)NULL)))? 1 : 0);
	}
}
// System.String Grpc.Core.AuthContext::get_PeerIdentityPropertyName()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* AuthContext_get_PeerIdentityPropertyName_m93D5A70893B0E101330D572340A0D9154B039655 (AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___peerIdentityPropertyName_0;
		return L_0;
	}
}
// System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty> Grpc.Core.AuthContext::get_PeerIdentity()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* AuthContext_get_PeerIdentity_m3240E725CF444449AF1D67D56458BF0ECE044078 (AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Dictionary_2_get_Item_mCC86C07EAC13D2EC7C63C99AB267F4A9B0BCBA7E_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		String_t* L_0 = __this->___peerIdentityPropertyName_0;
		if (L_0)
		{
			goto IL_000e;
		}
	}
	{
		RuntimeObject* L_1;
		L_1 = Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_inline(Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_RuntimeMethod_var);
		return L_1;
	}

IL_000e:
	{
		Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* L_2 = __this->___properties_1;
		String_t* L_3 = __this->___peerIdentityPropertyName_0;
		NullCheck(L_2);
		List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A* L_4;
		L_4 = Dictionary_2_get_Item_mCC86C07EAC13D2EC7C63C99AB267F4A9B0BCBA7E(L_2, L_3, Dictionary_2_get_Item_mCC86C07EAC13D2EC7C63C99AB267F4A9B0BCBA7E_RuntimeMethod_var);
		return L_4;
	}
}
// System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty> Grpc.Core.AuthContext::get_Properties()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* AuthContext_get_Properties_m9E26D09031068D5C1DB532492ED2A454F7DC3145 (AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Dictionary_2_get_Values_m23DB66E8602DED9744E45784C6E3A2F2C8F513D4_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Enumerable_SelectMany_TisList_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mEA8E736006D5A38482763344EB5EF9B911A7DDFA_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&U3CU3Ec_U3Cget_PropertiesU3Eb__10_0_m0F85D3F37CE6E6128A2F35657DFC2B85FC2EC928_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* G_B2_0 = NULL;
	ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010* G_B2_1 = NULL;
	Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* G_B1_0 = NULL;
	ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010* G_B1_1 = NULL;
	{
		Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* L_0 = __this->___properties_1;
		NullCheck(L_0);
		ValueCollection_tF108D37F3C0636938CE2C3F08C88F0464515C010* L_1;
		L_1 = Dictionary_2_get_Values_m23DB66E8602DED9744E45784C6E3A2F2C8F513D4(L_0, Dictionary_2_get_Values_m23DB66E8602DED9744E45784C6E3A2F2C8F513D4_RuntimeMethod_var);
		il2cpp_codegen_runtime_class_init_inline(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var);
		Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* L_2 = ((U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_StaticFields*)il2cpp_codegen_static_fields_for(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var))->___U3CU3E9__10_0_1;
		Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* L_3 = L_2;
		G_B1_0 = L_3;
		G_B1_1 = L_1;
		if (L_3)
		{
			G_B2_0 = L_3;
			G_B2_1 = L_1;
			goto IL_002a;
		}
	}
	{
		il2cpp_codegen_runtime_class_init_inline(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var);
		U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677* L_4 = ((U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_StaticFields*)il2cpp_codegen_static_fields_for(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var))->___U3CU3E9_0;
		Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* L_5 = (Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A*)il2cpp_codegen_object_new(Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A_il2cpp_TypeInfo_var);
		NullCheck(L_5);
		Func_2__ctor_m53DF15B454B7C7C541DE99174419A101E140AB51(L_5, L_4, (intptr_t)((void*)U3CU3Ec_U3Cget_PropertiesU3Eb__10_0_m0F85D3F37CE6E6128A2F35657DFC2B85FC2EC928_RuntimeMethod_var), NULL);
		Func_2_tE6E81DC5A7DA634ABEE5F47F0A355C81EB529E8A* L_6 = L_5;
		((U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_StaticFields*)il2cpp_codegen_static_fields_for(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var))->___U3CU3E9__10_0_1 = L_6;
		Il2CppCodeGenWriteBarrier((void**)(&((U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_StaticFields*)il2cpp_codegen_static_fields_for(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var))->___U3CU3E9__10_0_1), (void*)L_6);
		G_B2_0 = L_6;
		G_B2_1 = G_B1_1;
	}

IL_002a:
	{
		RuntimeObject* L_7;
		L_7 = Enumerable_SelectMany_TisList_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mEA8E736006D5A38482763344EB5EF9B911A7DDFA(G_B2_1, G_B2_0, Enumerable_SelectMany_TisList_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mEA8E736006D5A38482763344EB5EF9B911A7DDFA_RuntimeMethod_var);
		return L_7;
	}
}
// System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty> Grpc.Core.AuthContext::FindPropertiesByName(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* AuthContext_FindPropertiesByName_mE4E76EF52112BB33067FD223C34D1F82899C7D77 (AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* __this, String_t* ___0_propertyName, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Dictionary_2_TryGetValue_m06D597E171BEBB18AC362AE27BBD7BEE0A0C4F7F_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A* V_0 = NULL;
	{
		Dictionary_2_t38F9C496A32DDE11BF14F7BF1DC96150F51E0E50* L_0 = __this->___properties_1;
		String_t* L_1 = ___0_propertyName;
		NullCheck(L_0);
		bool L_2;
		L_2 = Dictionary_2_TryGetValue_m06D597E171BEBB18AC362AE27BBD7BEE0A0C4F7F(L_0, L_1, (&V_0), Dictionary_2_TryGetValue_m06D597E171BEBB18AC362AE27BBD7BEE0A0C4F7F_RuntimeMethod_var);
		if (L_2)
		{
			goto IL_0016;
		}
	}
	{
		RuntimeObject* L_3;
		L_3 = Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_inline(Enumerable_Empty_TisAuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_mDEE208690CADB17F5F5D26E4B5709C878E73DCC4_RuntimeMethod_var);
		return L_3;
	}

IL_0016:
	{
		List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A* L_4 = V_0;
		return L_4;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.AuthContext/<>c::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CU3Ec__cctor_m0627ED2D1D30E5CE19EC03AF1ED9A573E125CFA8 (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677* L_0 = (U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677*)il2cpp_codegen_object_new(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		U3CU3Ec__ctor_m6285DC63E7911C62ED2A49AB2A494C45FCEF0C31(L_0, NULL);
		((U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_StaticFields*)il2cpp_codegen_static_fields_for(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var))->___U3CU3E9_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&((U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_StaticFields*)il2cpp_codegen_static_fields_for(U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677_il2cpp_TypeInfo_var))->___U3CU3E9_0), (void*)L_0);
		return;
	}
}
// System.Void Grpc.Core.AuthContext/<>c::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CU3Ec__ctor_m6285DC63E7911C62ED2A49AB2A494C45FCEF0C31 (U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
// System.Collections.Generic.IEnumerable`1<Grpc.Core.AuthProperty> Grpc.Core.AuthContext/<>c::<get_Properties>b__10_0(System.Collections.Generic.List`1<Grpc.Core.AuthProperty>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* U3CU3Ec_U3Cget_PropertiesU3Eb__10_0_m0F85D3F37CE6E6128A2F35657DFC2B85FC2EC928 (U3CU3Ec_t01FD4429FE0A359B2D1A6DEBAFD4B024FE8C4677* __this, List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A* ___0_v, const RuntimeMethod* method) 
{
	{
		List_1_t0EC04368A74FDAE9994A9F6ECD9D17830197978A* L_0 = ___0_v;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.AuthProperty::.ctor(System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AuthProperty__ctor_mDEECB7A796ACFFF79FE6EAAD654310264613386E (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* __this, String_t* ___0_name, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_valueBytes, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_name;
		String_t* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9(L_0, GrpcPreconditions_CheckNotNull_TisString_t_mE2792F2165239FD5DCD97F939315D29166DBA5D9_RuntimeMethod_var);
		__this->___name_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___name_1), (void*)L_1);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_2 = ___1_valueBytes;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_3;
		L_3 = GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059(L_2, GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059_RuntimeMethod_var);
		__this->___valueBytes_2 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___valueBytes_2), (void*)L_3);
		return;
	}
}
// System.String Grpc.Core.AuthProperty::get_Name()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* AuthProperty_get_Name_m7E46BB76C5C6385FD3E1011CB81A4C5D874068B9 (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___name_1;
		return L_0;
	}
}
// System.String Grpc.Core.AuthProperty::get_Value()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* AuthProperty_get_Value_mCA319059001BDCB96F6C9BD673EF38AAD9993526 (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	String_t* V_0 = NULL;
	String_t* G_B2_0 = NULL;
	String_t* G_B1_0 = NULL;
	{
		String_t* L_0 = __this->___lazyValue_3;
		String_t* L_1 = L_0;
		G_B1_0 = L_1;
		if (L_1)
		{
			G_B2_0 = L_1;
			goto IL_0023;
		}
	}
	{
		il2cpp_codegen_runtime_class_init_inline(AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_2 = ((AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_StaticFields*)il2cpp_codegen_static_fields_for(AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var))->___EncodingUTF8_0;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_3 = __this->___valueBytes_2;
		NullCheck(L_2);
		String_t* L_4;
		L_4 = VirtualFuncInvoker1< String_t*, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* >::Invoke(34 /* System.String System.Text.Encoding::GetString(System.Byte[]) */, L_2, L_3);
		String_t* L_5 = L_4;
		V_0 = L_5;
		__this->___lazyValue_3 = L_5;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___lazyValue_3), (void*)L_5);
		String_t* L_6 = V_0;
		G_B2_0 = L_6;
	}

IL_0023:
	{
		return G_B2_0;
	}
}
// System.Byte[] Grpc.Core.AuthProperty::get_ValueBytes()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* AuthProperty_get_ValueBytes_m2343576299BA1DBF437DD756F4D74A9B77F4A0A9 (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* V_0 = NULL;
	{
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = __this->___valueBytes_2;
		NullCheck(L_0);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_1 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)SZArrayNew(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var, (uint32_t)((int32_t)(((RuntimeArray*)L_0)->max_length)));
		V_0 = L_1;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_2 = __this->___valueBytes_2;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_3 = V_0;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_4 = __this->___valueBytes_2;
		NullCheck(L_4);
		Buffer_BlockCopy_m2F7BC0C5BA97C500E3F87D5008718F797E02B358((RuntimeArray*)L_2, 0, (RuntimeArray*)L_3, 0, ((int32_t)(((RuntimeArray*)L_4)->max_length)), NULL);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_5 = V_0;
		return L_5;
	}
}
// Grpc.Core.AuthProperty Grpc.Core.AuthProperty::Create(System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* AuthProperty_Create_m2A3962EA219F735E1697609C9CAEB8C3333EC115 (String_t* ___0_name, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_valueBytes, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* V_0 = NULL;
	{
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = ___1_valueBytes;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059(L_0, GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_mD0107EC48E155C724B228B4013B4316A2676E059_RuntimeMethod_var);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_2 = ___1_valueBytes;
		NullCheck(L_2);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_3 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)SZArrayNew(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var, (uint32_t)((int32_t)(((RuntimeArray*)L_2)->max_length)));
		V_0 = L_3;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_4 = ___1_valueBytes;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_5 = V_0;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_6 = ___1_valueBytes;
		NullCheck(L_6);
		Buffer_BlockCopy_m2F7BC0C5BA97C500E3F87D5008718F797E02B358((RuntimeArray*)L_4, 0, (RuntimeArray*)L_5, 0, ((int32_t)(((RuntimeArray*)L_6)->max_length)), NULL);
		String_t* L_7 = ___0_name;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_8 = V_0;
		AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* L_9 = (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF*)il2cpp_codegen_object_new(AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var);
		NullCheck(L_9);
		AuthProperty__ctor_mDEECB7A796ACFFF79FE6EAAD654310264613386E(L_9, L_7, L_8, NULL);
		return L_9;
	}
}
// System.Byte[] Grpc.Core.AuthProperty::get_ValueBytesUnsafe()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* AuthProperty_get_ValueBytesUnsafe_m7A70AF37788D4468DCC828ED41FA1B907BBA6C00 (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* __this, const RuntimeMethod* method) 
{
	{
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = __this->___valueBytes_2;
		return L_0;
	}
}
// Grpc.Core.AuthProperty Grpc.Core.AuthProperty::CreateUnsafe(System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* AuthProperty_CreateUnsafe_m3AEAFD144CFC79D571497EC1A61E360613B5AF27 (String_t* ___0_name, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_valueBytes, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		String_t* L_0 = ___0_name;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_1 = ___1_valueBytes;
		AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF* L_2 = (AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF*)il2cpp_codegen_object_new(AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		AuthProperty__ctor_mDEECB7A796ACFFF79FE6EAAD654310264613386E(L_2, L_0, L_1, NULL);
		return L_2;
	}
}
// System.Void Grpc.Core.AuthProperty::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AuthProperty__cctor_mB7C0897CEB7E404DBBAD660C00C2813EF504C49F (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_0;
		L_0 = Encoding_get_UTF8_m9FA98A53CE96FD6D02982625C5246DD36C1235C9(NULL);
		((AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_StaticFields*)il2cpp_codegen_static_fields_for(AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var))->___EncodingUTF8_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&((AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_StaticFields*)il2cpp_codegen_static_fields_for(AuthProperty_tC8B968201548714D2BC2BC4F25193060A8C8D6BF_il2cpp_TypeInfo_var))->___EncodingUTF8_0), (void*)L_0);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.BindServiceMethodAttribute::.ctor(System.Type,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void BindServiceMethodAttribute__ctor_m3D47DBF674BB18692C629F845F20FE463B3A3129 (BindServiceMethodAttribute_tA0C4802BE18E8C3D565943379450CB84CDEE2CDA* __this, Type_t* ___0_bindType, String_t* ___1_bindMethodName, const RuntimeMethod* method) 
{
	{
		Attribute__ctor_m79ED1BF1EE36D1E417BA89A0D9F91F8AAD8D19E2(__this, NULL);
		Type_t* L_0 = ___0_bindType;
		__this->___U3CBindTypeU3Ek__BackingField_1 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___U3CBindTypeU3Ek__BackingField_1), (void*)L_0);
		String_t* L_1 = ___1_bindMethodName;
		__this->___U3CBindMethodNameU3Ek__BackingField_2 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___U3CBindMethodNameU3Ek__BackingField_2), (void*)L_1);
		return;
	}
}
// System.Type Grpc.Core.BindServiceMethodAttribute::get_BindType()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Type_t* BindServiceMethodAttribute_get_BindType_m79B61D9E85034B5A1CBFF04329E7CA18065ED865 (BindServiceMethodAttribute_tA0C4802BE18E8C3D565943379450CB84CDEE2CDA* __this, const RuntimeMethod* method) 
{
	{
		Type_t* L_0 = __this->___U3CBindTypeU3Ek__BackingField_1;
		return L_0;
	}
}
// System.String Grpc.Core.BindServiceMethodAttribute::get_BindMethodName()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* BindServiceMethodAttribute_get_BindMethodName_mC325C6CCEC36C9D71F49F8D6AE6A8EE3C1448C47 (BindServiceMethodAttribute_tA0C4802BE18E8C3D565943379450CB84CDEE2CDA* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___U3CBindMethodNameU3Ek__BackingField_2;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// Grpc.Core.CallCredentials Grpc.Core.CallCredentials::Compose(Grpc.Core.CallCredentials[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* CallCredentials_Compose_mE828E219CCACCAF2C3D20ACC86D6F2B0C2C8523F (CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* ___0_credentials, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* L_0 = ___0_credentials;
		CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E* L_1 = (CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E*)il2cpp_codegen_object_new(CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		CompositeCallCredentials__ctor_m9900E80BCCAC873F8E1363C3FF4075DC81CF9E0E(L_1, L_0, NULL);
		return L_1;
	}
}
// Grpc.Core.CallCredentials Grpc.Core.CallCredentials::FromInterceptor(Grpc.Core.AsyncAuthInterceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* CallCredentials_FromInterceptor_m5646C6EB7D10FD9F624BA26C2A697796AB1D59E4 (AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* ___0_interceptor, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* L_0 = ___0_interceptor;
		AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862* L_1 = (AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862*)il2cpp_codegen_object_new(AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		AsyncAuthInterceptorCredentials__ctor_m71EBDE4E90210DDFC681326E27CA4EF245769D69(L_1, L_0, NULL);
		return L_1;
	}
}
// System.Void Grpc.Core.CallCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CallCredentials__ctor_m88A74796A15DA1B237649AE2F738E8F4834B7141 (CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.CallCredentials/CompositeCallCredentials::.ctor(Grpc.Core.CallCredentials[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CompositeCallCredentials__ctor_m9900E80BCCAC873F8E1363C3FF4075DC81CF9E0E (CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E* __this, CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* ___0_credentials, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_AsReadOnly_m730830764F278CFE6CD6A5E39836BFD8BED58AED_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1__ctor_m5488D09159AC2D56E9F9779F44ADF0DD0E684049_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralFF5D592BA64F7E8E842A9A677126A31DF0EEBCDE);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallCredentials__ctor_m88A74796A15DA1B237649AE2F738E8F4834B7141(__this, NULL);
		CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* L_0 = ___0_credentials;
		NullCheck(L_0);
		GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497((bool)((((int32_t)((((int32_t)((int32_t)(((RuntimeArray*)L_0)->max_length))) < ((int32_t)2))? 1 : 0)) == ((int32_t)0))? 1 : 0), _stringLiteralFF5D592BA64F7E8E842A9A677126A31DF0EEBCDE, NULL);
		CallCredentialsU5BU5D_t7FA538664F30E4021A71D9CBA62706566CA070BD* L_1 = ___0_credentials;
		List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1* L_2 = (List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1*)il2cpp_codegen_object_new(List_1_tAF05705D4489E6CD1151FA0D6C17E2BA7267C4B1_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		List_1__ctor_m5488D09159AC2D56E9F9779F44ADF0DD0E684049(L_2, (RuntimeObject*)L_1, List_1__ctor_m5488D09159AC2D56E9F9779F44ADF0DD0E684049_RuntimeMethod_var);
		NullCheck(L_2);
		ReadOnlyCollection_1_tC203378F06B8F145A48BFB0A1546E72C1B700445* L_3;
		L_3 = List_1_AsReadOnly_m730830764F278CFE6CD6A5E39836BFD8BED58AED(L_2, List_1_AsReadOnly_m730830764F278CFE6CD6A5E39836BFD8BED58AED_RuntimeMethod_var);
		__this->___credentials_0 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___credentials_0), (void*)L_3);
		return;
	}
}
// System.Void Grpc.Core.CallCredentials/CompositeCallCredentials::InternalPopulateConfiguration(Grpc.Core.CallCredentialsConfiguratorBase,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CompositeCallCredentials_InternalPopulateConfiguration_m3275D553172B1B3B79A40E44246F5649A6EC3B2F (CompositeCallCredentials_t1061BCEC9DCC63C68E35C6E7D26A99D7B93A588E* __this, CallCredentialsConfiguratorBase_t90FECA680004ADF081D728A5765058B4643E0078* ___0_configurator, RuntimeObject* ___1_state, const RuntimeMethod* method) 
{
	{
		CallCredentialsConfiguratorBase_t90FECA680004ADF081D728A5765058B4643E0078* L_0 = ___0_configurator;
		RuntimeObject* L_1 = ___1_state;
		RuntimeObject* L_2 = __this->___credentials_0;
		NullCheck(L_0);
		VirtualActionInvoker2< RuntimeObject*, RuntimeObject* >::Invoke(4 /* System.Void Grpc.Core.CallCredentialsConfiguratorBase::SetCompositeCredentials(System.Object,System.Collections.Generic.IReadOnlyList`1<Grpc.Core.CallCredentials>) */, L_0, L_1, L_2);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials::.ctor(Grpc.Core.AsyncAuthInterceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncAuthInterceptorCredentials__ctor_m71EBDE4E90210DDFC681326E27CA4EF245769D69 (AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862* __this, AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* ___0_interceptor, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisAsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968_m0512535AE0D48827C37B1D3A08E71D0AE57FEE20_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallCredentials__ctor_m88A74796A15DA1B237649AE2F738E8F4834B7141(__this, NULL);
		AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* L_0 = ___0_interceptor;
		AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisAsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968_m0512535AE0D48827C37B1D3A08E71D0AE57FEE20(L_0, GrpcPreconditions_CheckNotNull_TisAsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968_m0512535AE0D48827C37B1D3A08E71D0AE57FEE20_RuntimeMethod_var);
		__this->___interceptor_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___interceptor_0), (void*)L_1);
		return;
	}
}
// System.Void Grpc.Core.CallCredentials/AsyncAuthInterceptorCredentials::InternalPopulateConfiguration(Grpc.Core.CallCredentialsConfiguratorBase,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void AsyncAuthInterceptorCredentials_InternalPopulateConfiguration_mB41D6686A5CBFAE9B663CDBC63F4BC1CFF4645D1 (AsyncAuthInterceptorCredentials_t873AA13F2A99E53031C0C6E5D568D2FD31DA3862* __this, CallCredentialsConfiguratorBase_t90FECA680004ADF081D728A5765058B4643E0078* ___0_configurator, RuntimeObject* ___1_state, const RuntimeMethod* method) 
{
	{
		CallCredentialsConfiguratorBase_t90FECA680004ADF081D728A5765058B4643E0078* L_0 = ___0_configurator;
		RuntimeObject* L_1 = ___1_state;
		AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* L_2 = __this->___interceptor_0;
		NullCheck(L_0);
		VirtualActionInvoker2< RuntimeObject*, AsyncAuthInterceptor_t065ED29F843FDD1CC730772CACDEBB5A49351968* >::Invoke(5 /* System.Void Grpc.Core.CallCredentialsConfiguratorBase::SetAsyncAuthInterceptorCredentials(System.Object,Grpc.Core.AsyncAuthInterceptor) */, L_0, L_1, L_2);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.CallCredentialsConfiguratorBase::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CallCredentialsConfiguratorBase__ctor_mA7C79537C6E0AB578766D0AFE213EC4E983F4BF2 (CallCredentialsConfiguratorBase_t90FECA680004ADF081D728A5765058B4643E0078* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.CallInvoker::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CallInvoker__ctor_mFFDA4B00606F063F1EAC9DBBCDDECD9F82705E6B (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif


// Conversion methods for marshalling of: Grpc.Core.CallOptions
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_pinvoke(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke& marshaled)
{
	Exception_t* ___headers_0Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'headers' of type 'CallOptions': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___headers_0Exception, NULL);
}
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_pinvoke_back(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke& marshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled)
{
	Exception_t* ___headers_0Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'headers' of type 'CallOptions': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___headers_0Exception, NULL);
}
// Conversion method for clean up from marshalling of: Grpc.Core.CallOptions
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_pinvoke_cleanup(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_pinvoke& marshaled)
{
}


// Conversion methods for marshalling of: Grpc.Core.CallOptions
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_com(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com& marshaled)
{
	Exception_t* ___headers_0Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'headers' of type 'CallOptions': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___headers_0Exception, NULL);
}
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_com_back(const CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com& marshaled, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E& unmarshaled)
{
	Exception_t* ___headers_0Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'headers' of type 'CallOptions': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___headers_0Exception, NULL);
}
// Conversion method for clean up from marshalling of: Grpc.Core.CallOptions
IL2CPP_EXTERN_C void CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshal_com_cleanup(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E_marshaled_com& marshaled)
{
}
// System.Void Grpc.Core.CallOptions::.ctor(Grpc.Core.Metadata,System.Nullable`1<System.DateTime>,System.Threading.CancellationToken,Grpc.Core.WriteOptions,Grpc.Core.ContextPropagationToken,Grpc.Core.CallCredentials)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CallOptions__ctor_m3CA2A09D88B01048E1C33BAA895BD8133F098CA2 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_headers, Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC ___1_deadline, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ___2_cancellationToken, WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___3_writeOptions, ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___4_propagationToken, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___5_credentials, const RuntimeMethod* method) 
{
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0 = ___0_headers;
		__this->___headers_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___headers_0), (void*)L_0);
		Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC L_1 = ___1_deadline;
		__this->___deadline_1 = L_1;
		CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED L_2 = ___2_cancellationToken;
		__this->___cancellationToken_2 = L_2;
		Il2CppCodeGenWriteBarrier((void**)&(((&__this->___cancellationToken_2))->____source_0), (void*)NULL);
		WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* L_3 = ___3_writeOptions;
		__this->___writeOptions_3 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___writeOptions_3), (void*)L_3);
		ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* L_4 = ___4_propagationToken;
		__this->___propagationToken_4 = L_4;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___propagationToken_4), (void*)L_4);
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_5 = ___5_credentials;
		__this->___credentials_5 = L_5;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___credentials_5), (void*)L_5);
		__this->___flags_6 = 0;
		return;
	}
}
IL2CPP_EXTERN_C  void CallOptions__ctor_m3CA2A09D88B01048E1C33BAA895BD8133F098CA2_AdjustorThunk (RuntimeObject* __this, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_headers, Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC ___1_deadline, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ___2_cancellationToken, WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___3_writeOptions, ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___4_propagationToken, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___5_credentials, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions__ctor_m3CA2A09D88B01048E1C33BAA895BD8133F098CA2(_thisAdjusted, ___0_headers, ___1_deadline, ___2_cancellationToken, ___3_writeOptions, ___4_propagationToken, ___5_credentials, method);
}
// Grpc.Core.Metadata Grpc.Core.CallOptions::get_Headers()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* CallOptions_get_Headers_mC83DA74FDC549907063A89E8E3589E244EE244BB (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0 = __this->___headers_0;
		return L_0;
	}
}
IL2CPP_EXTERN_C  Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* CallOptions_get_Headers_mC83DA74FDC549907063A89E8E3589E244EE244BB_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* _returnValue;
	_returnValue = CallOptions_get_Headers_mC83DA74FDC549907063A89E8E3589E244EE244BB_inline(_thisAdjusted, method);
	return _returnValue;
}
// System.Nullable`1<System.DateTime> Grpc.Core.CallOptions::get_Deadline()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC CallOptions_get_Deadline_m2B9B9FB43004CBA8DE9B7BFE125991B4FB24EEDA (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC L_0 = __this->___deadline_1;
		return L_0;
	}
}
IL2CPP_EXTERN_C  Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC CallOptions_get_Deadline_m2B9B9FB43004CBA8DE9B7BFE125991B4FB24EEDA_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC _returnValue;
	_returnValue = CallOptions_get_Deadline_m2B9B9FB43004CBA8DE9B7BFE125991B4FB24EEDA_inline(_thisAdjusted, method);
	return _returnValue;
}
// System.Threading.CancellationToken Grpc.Core.CallOptions::get_CancellationToken()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED CallOptions_get_CancellationToken_m26B31F9F5DDE686ED39260681F29301AA878068B (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED L_0 = __this->___cancellationToken_2;
		return L_0;
	}
}
IL2CPP_EXTERN_C  CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED CallOptions_get_CancellationToken_m26B31F9F5DDE686ED39260681F29301AA878068B_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED _returnValue;
	_returnValue = CallOptions_get_CancellationToken_m26B31F9F5DDE686ED39260681F29301AA878068B_inline(_thisAdjusted, method);
	return _returnValue;
}
// Grpc.Core.WriteOptions Grpc.Core.CallOptions::get_WriteOptions()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* CallOptions_get_WriteOptions_m7F6A4E257CA22FD8E4D3FA0633DACABA5B95817E (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* L_0 = __this->___writeOptions_3;
		return L_0;
	}
}
IL2CPP_EXTERN_C  WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* CallOptions_get_WriteOptions_m7F6A4E257CA22FD8E4D3FA0633DACABA5B95817E_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* _returnValue;
	_returnValue = CallOptions_get_WriteOptions_m7F6A4E257CA22FD8E4D3FA0633DACABA5B95817E_inline(_thisAdjusted, method);
	return _returnValue;
}
// Grpc.Core.ContextPropagationToken Grpc.Core.CallOptions::get_PropagationToken()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* CallOptions_get_PropagationToken_m6FAE4454FD0AC717C9DFA54255ACCDA9D834C522 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* L_0 = __this->___propagationToken_4;
		return L_0;
	}
}
IL2CPP_EXTERN_C  ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* CallOptions_get_PropagationToken_m6FAE4454FD0AC717C9DFA54255ACCDA9D834C522_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* _returnValue;
	_returnValue = CallOptions_get_PropagationToken_m6FAE4454FD0AC717C9DFA54255ACCDA9D834C522_inline(_thisAdjusted, method);
	return _returnValue;
}
// Grpc.Core.CallCredentials Grpc.Core.CallOptions::get_Credentials()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* CallOptions_get_Credentials_m36616F8A973A7EC538A623897CC7A0A832BACF9F (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_0 = __this->___credentials_5;
		return L_0;
	}
}
IL2CPP_EXTERN_C  CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* CallOptions_get_Credentials_m36616F8A973A7EC538A623897CC7A0A832BACF9F_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* _returnValue;
	_returnValue = CallOptions_get_Credentials_m36616F8A973A7EC538A623897CC7A0A832BACF9F_inline(_thisAdjusted, method);
	return _returnValue;
}
// System.Boolean Grpc.Core.CallOptions::get_IsWaitForReady()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool CallOptions_get_IsWaitForReady_m5F54CD453B786EA853E52B700D7D1BCD403DA4C4 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->___flags_6;
		return (bool)((((int32_t)((int32_t)((int32_t)L_0&((int32_t)32)))) == ((int32_t)((int32_t)32)))? 1 : 0);
	}
}
IL2CPP_EXTERN_C  bool CallOptions_get_IsWaitForReady_m5F54CD453B786EA853E52B700D7D1BCD403DA4C4_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	bool _returnValue;
	_returnValue = CallOptions_get_IsWaitForReady_m5F54CD453B786EA853E52B700D7D1BCD403DA4C4(_thisAdjusted, method);
	return _returnValue;
}
// Grpc.Core.Internal.CallFlags Grpc.Core.CallOptions::get_Flags()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t CallOptions_get_Flags_mA4C453704C2AC0C8E04BE00C588ACE4E1F8CC969 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->___flags_6;
		return L_0;
	}
}
IL2CPP_EXTERN_C  int32_t CallOptions_get_Flags_mA4C453704C2AC0C8E04BE00C588ACE4E1F8CC969_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	int32_t _returnValue;
	_returnValue = CallOptions_get_Flags_mA4C453704C2AC0C8E04BE00C588ACE4E1F8CC969_inline(_thisAdjusted, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithHeaders(Grpc.Core.Metadata)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithHeaders_m5AEB13430BD0210D3BC3D247015728F3417FF82B (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_headers, const RuntimeMethod* method) 
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_0 = (*(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*)__this);
		V_0 = L_0;
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_1 = ___0_headers;
		(&V_0)->___headers_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&(&V_0)->___headers_0), (void*)L_1);
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_2 = V_0;
		return L_2;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithHeaders_m5AEB13430BD0210D3BC3D247015728F3417FF82B_AdjustorThunk (RuntimeObject* __this, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_headers, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithHeaders_m5AEB13430BD0210D3BC3D247015728F3417FF82B(_thisAdjusted, ___0_headers, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithDeadline(System.DateTime)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithDeadline_mA865862C3CF382BC7493F8A4F7102476CF61A44A (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___0_deadline, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Nullable_1__ctor_mB17304720EA19F5469A4883827F53A75FEB492CF_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_0 = (*(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*)__this);
		V_0 = L_0;
		DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D L_1 = ___0_deadline;
		Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC L_2;
		memset((&L_2), 0, sizeof(L_2));
		Nullable_1__ctor_mB17304720EA19F5469A4883827F53A75FEB492CF((&L_2), L_1, /*hidden argument*/Nullable_1__ctor_mB17304720EA19F5469A4883827F53A75FEB492CF_RuntimeMethod_var);
		(&V_0)->___deadline_1 = L_2;
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_3 = V_0;
		return L_3;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithDeadline_mA865862C3CF382BC7493F8A4F7102476CF61A44A_AdjustorThunk (RuntimeObject* __this, DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ___0_deadline, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithDeadline_mA865862C3CF382BC7493F8A4F7102476CF61A44A(_thisAdjusted, ___0_deadline, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithCancellationToken(System.Threading.CancellationToken)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithCancellationToken_m1F875E2F1757A6AE7E1205096C33DE8F1E795576 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ___0_cancellationToken, const RuntimeMethod* method) 
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_0 = (*(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*)__this);
		V_0 = L_0;
		CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED L_1 = ___0_cancellationToken;
		(&V_0)->___cancellationToken_2 = L_1;
		Il2CppCodeGenWriteBarrier((void**)&(((&(&V_0)->___cancellationToken_2))->____source_0), (void*)NULL);
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_2 = V_0;
		return L_2;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithCancellationToken_m1F875E2F1757A6AE7E1205096C33DE8F1E795576_AdjustorThunk (RuntimeObject* __this, CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ___0_cancellationToken, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithCancellationToken_m1F875E2F1757A6AE7E1205096C33DE8F1E795576(_thisAdjusted, ___0_cancellationToken, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithWriteOptions(Grpc.Core.WriteOptions)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithWriteOptions_mAB5238A794F9B29D1F1D8222C1C4C233812FD1B8 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___0_writeOptions, const RuntimeMethod* method) 
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_0 = (*(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*)__this);
		V_0 = L_0;
		WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* L_1 = ___0_writeOptions;
		(&V_0)->___writeOptions_3 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&(&V_0)->___writeOptions_3), (void*)L_1);
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_2 = V_0;
		return L_2;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithWriteOptions_mAB5238A794F9B29D1F1D8222C1C4C233812FD1B8_AdjustorThunk (RuntimeObject* __this, WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___0_writeOptions, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithWriteOptions_mAB5238A794F9B29D1F1D8222C1C4C233812FD1B8(_thisAdjusted, ___0_writeOptions, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithPropagationToken(Grpc.Core.ContextPropagationToken)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithPropagationToken_m483DBA0E2928ACEB07184F87ACDA7F1E98C16248 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___0_propagationToken, const RuntimeMethod* method) 
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_0 = (*(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*)__this);
		V_0 = L_0;
		ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* L_1 = ___0_propagationToken;
		(&V_0)->___propagationToken_4 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&(&V_0)->___propagationToken_4), (void*)L_1);
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_2 = V_0;
		return L_2;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithPropagationToken_m483DBA0E2928ACEB07184F87ACDA7F1E98C16248_AdjustorThunk (RuntimeObject* __this, ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ___0_propagationToken, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithPropagationToken_m483DBA0E2928ACEB07184F87ACDA7F1E98C16248(_thisAdjusted, ___0_propagationToken, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithCredentials(Grpc.Core.CallCredentials)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithCredentials_m7F68A03E4A1A680D956A13163E4DD90172F80D4E (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___0_credentials, const RuntimeMethod* method) 
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_0 = (*(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*)__this);
		V_0 = L_0;
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_1 = ___0_credentials;
		(&V_0)->___credentials_5 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&(&V_0)->___credentials_5), (void*)L_1);
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_2 = V_0;
		return L_2;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithCredentials_m7F68A03E4A1A680D956A13163E4DD90172F80D4E_AdjustorThunk (RuntimeObject* __this, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___0_credentials, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithCredentials_m7F68A03E4A1A680D956A13163E4DD90172F80D4E(_thisAdjusted, ___0_credentials, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithWaitForReady(System.Boolean)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithWaitForReady_mAD0AD13D6E4D0289DCDB2F205A6074E7454D6A71 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, bool ___0_waitForReady, const RuntimeMethod* method) 
{
	{
		bool L_0 = ___0_waitForReady;
		if (!L_0)
		{
			goto IL_0013;
		}
	}
	{
		int32_t L_1 = __this->___flags_6;
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_2;
		L_2 = CallOptions_WithFlags_mBF19C96B91674EF7022CDEDF071C21788B231CB5(__this, ((int32_t)((int32_t)L_1|((int32_t)32))), NULL);
		return L_2;
	}

IL_0013:
	{
		int32_t L_3 = __this->___flags_6;
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_4;
		L_4 = CallOptions_WithFlags_mBF19C96B91674EF7022CDEDF071C21788B231CB5(__this, ((int32_t)((int32_t)L_3&((int32_t)-33))), NULL);
		return L_4;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithWaitForReady_mAD0AD13D6E4D0289DCDB2F205A6074E7454D6A71_AdjustorThunk (RuntimeObject* __this, bool ___0_waitForReady, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithWaitForReady_mAD0AD13D6E4D0289DCDB2F205A6074E7454D6A71(_thisAdjusted, ___0_waitForReady, method);
	return _returnValue;
}
// Grpc.Core.CallOptions Grpc.Core.CallOptions::WithFlags(Grpc.Core.Internal.CallFlags)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithFlags_mBF19C96B91674EF7022CDEDF071C21788B231CB5 (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, int32_t ___0_flags, const RuntimeMethod* method) 
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_0 = (*(CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*)__this);
		V_0 = L_0;
		int32_t L_1 = ___0_flags;
		(&V_0)->___flags_6 = L_1;
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_2 = V_0;
		return L_2;
	}
}
IL2CPP_EXTERN_C  CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E CallOptions_WithFlags_mBF19C96B91674EF7022CDEDF071C21788B231CB5_AdjustorThunk (RuntimeObject* __this, int32_t ___0_flags, const RuntimeMethod* method)
{
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E*>(__this + _offset);
	CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E _returnValue;
	_returnValue = CallOptions_WithFlags_mBF19C96B91674EF7022CDEDF071C21788B231CB5(_thisAdjusted, ___0_flags, method);
	return _returnValue;
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ChannelBase::.ctor(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ChannelBase__ctor_mDDE4C851EF19403CB9DC1F7734DE933C02CC3F94 (ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* __this, String_t* ___0_target, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral320772EF40302B49A179DB96BAD02224E97B4018);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_target;
		String_t* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B(L_0, _stringLiteral320772EF40302B49A179DB96BAD02224E97B4018, GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		__this->___target_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___target_0), (void*)L_1);
		return;
	}
}
// System.String Grpc.Core.ChannelBase::get_Target()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* ChannelBase_get_Target_mBFE57BC2288894FC07F978DE848DFF3C68253CEA (ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___target_0;
		return L_0;
	}
}
// System.Threading.Tasks.Task Grpc.Core.ChannelBase::ShutdownAsync()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* ChannelBase_ShutdownAsync_mBC9CAA1B05198B97DEFED98C26065DC329680170 (ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* __this, const RuntimeMethod* method) 
{
	{
		Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* L_0;
		L_0 = VirtualFuncInvoker0< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* >::Invoke(5 /* System.Threading.Tasks.Task Grpc.Core.ChannelBase::ShutdownAsyncCore() */, __this);
		return L_0;
	}
}
// System.Threading.Tasks.Task Grpc.Core.ChannelBase::ShutdownAsyncCore()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* ChannelBase_ShutdownAsyncCore_mB982694BFA2DA450C97E37EFC9ED5625151D91B4 (ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&AsyncTaskMethodBuilder_Start_TisU3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44_m9121BD027CA131910ADCDDCFA9EBE759B4E9AD46_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44 V_0;
	memset((&V_0), 0, sizeof(V_0));
	{
		il2cpp_codegen_runtime_class_init_inline(AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06_il2cpp_TypeInfo_var);
		AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06 L_0;
		L_0 = AsyncTaskMethodBuilder_Create_m24F3F260A79CA2B9EC4F1F15693A33F5EC58735A(NULL);
		(&V_0)->___U3CU3Et__builder_1 = L_0;
		Il2CppCodeGenWriteBarrier((void**)&((&((&(((&(&V_0)->___U3CU3Et__builder_1))->___m_builder_1))->___m_coreState_1))->___m_stateMachine_0), (void*)NULL);
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&((&((&(((&(&V_0)->___U3CU3Et__builder_1))->___m_builder_1))->___m_coreState_1))->___m_defaultContextAction_1), (void*)NULL);
		#endif
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&((&(((&(&V_0)->___U3CU3Et__builder_1))->___m_builder_1))->___m_task_2), (void*)NULL);
		#endif
		(&V_0)->___U3CU3E1__state_0 = (-1);
		AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* L_1 = (&(&V_0)->___U3CU3Et__builder_1);
		AsyncTaskMethodBuilder_Start_TisU3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44_m9121BD027CA131910ADCDDCFA9EBE759B4E9AD46(L_1, (&V_0), AsyncTaskMethodBuilder_Start_TisU3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44_m9121BD027CA131910ADCDDCFA9EBE759B4E9AD46_RuntimeMethod_var);
		AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* L_2 = (&(&V_0)->___U3CU3Et__builder_1);
		Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* L_3;
		L_3 = AsyncTaskMethodBuilder_get_Task_mE9125D5F8B96F26D1BA5A5347ED82669521C7F9E(L_2, NULL);
		return L_3;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6::MoveNext()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CShutdownAsyncCoreU3Ed__6_MoveNext_m4018328719353F183D6B961B3EA66CF8E0E48304 (U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* __this, const RuntimeMethod* method) 
{
	Exception_t* V_0 = NULL;
	il2cpp::utils::ExceptionSupportStack<RuntimeObject*, 1> __active_exceptions;
	try
	{// begin try (depth: 1)
		goto IL_0019;
	}// end try (depth: 1)
	catch(Il2CppExceptionWrapper& e)
	{
		if(il2cpp_codegen_class_is_assignable_from (((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&Exception_t_il2cpp_TypeInfo_var)), il2cpp_codegen_object_class(e.ex)))
		{
			IL2CPP_PUSH_ACTIVE_EXCEPTION(e.ex);
			goto CATCH_0002;
		}
		throw e;
	}

CATCH_0002:
	{// begin catch(System.Exception)
		V_0 = ((Exception_t*)IL2CPP_GET_ACTIVE_EXCEPTION(Exception_t*));
		__this->___U3CU3E1__state_0 = ((int32_t)-2);
		AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* L_0 = (&__this->___U3CU3Et__builder_1);
		Exception_t* L_1 = V_0;
		AsyncTaskMethodBuilder_SetException_mBE41863F0571E0177A15731294087DE45E1FC10B(L_0, L_1, NULL);
		IL2CPP_POP_ACTIVE_EXCEPTION();
		goto IL_002c;
	}// end catch (depth: 1)

IL_0019:
	{
		__this->___U3CU3E1__state_0 = ((int32_t)-2);
		AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* L_2 = (&__this->___U3CU3Et__builder_1);
		AsyncTaskMethodBuilder_SetResult_m76D8B84F0068257C1823B1200B00E58E0C8DDDDE(L_2, NULL);
	}

IL_002c:
	{
		return;
	}
}
IL2CPP_EXTERN_C  void U3CShutdownAsyncCoreU3Ed__6_MoveNext_m4018328719353F183D6B961B3EA66CF8E0E48304_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44*>(__this + _offset);
	U3CShutdownAsyncCoreU3Ed__6_MoveNext_m4018328719353F183D6B961B3EA66CF8E0E48304(_thisAdjusted, method);
}
// System.Void Grpc.Core.ChannelBase/<ShutdownAsyncCore>d__6::SetStateMachine(System.Runtime.CompilerServices.IAsyncStateMachine)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CShutdownAsyncCoreU3Ed__6_SetStateMachine_m133828B5FD4D7C31A38471B78C3A0B4799E98864 (U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* __this, RuntimeObject* ___0_stateMachine, const RuntimeMethod* method) 
{
	{
		AsyncTaskMethodBuilder_t7A5128C134547B5918EB1AA24FE47ED4C1DF3F06* L_0 = (&__this->___U3CU3Et__builder_1);
		RuntimeObject* L_1 = ___0_stateMachine;
		AsyncTaskMethodBuilder_SetStateMachine_mE52B5B6B076025592A7AB462E3D26FA434AEB795(L_0, L_1, NULL);
		return;
	}
}
IL2CPP_EXTERN_C  void U3CShutdownAsyncCoreU3Ed__6_SetStateMachine_m133828B5FD4D7C31A38471B78C3A0B4799E98864_AdjustorThunk (RuntimeObject* __this, RuntimeObject* ___0_stateMachine, const RuntimeMethod* method)
{
	U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<U3CShutdownAsyncCoreU3Ed__6_t255408B0322614AB8487DDEFFF6A96E814F33A44*>(__this + _offset);
	U3CShutdownAsyncCoreU3Ed__6_SetStateMachine_m133828B5FD4D7C31A38471B78C3A0B4799E98864(_thisAdjusted, ___0_stateMachine, method);
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ChannelCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ChannelCredentials__ctor_m5BAB665085468AD860D966A9B09538F491150849 (ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
// Grpc.Core.ChannelCredentials Grpc.Core.ChannelCredentials::get_Insecure()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ChannelCredentials_get_Insecure_m8682EF8F5EF36B221517E8C557DCE5D8697EB7BD (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		il2cpp_codegen_runtime_class_init_inline(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_0 = ((ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_StaticFields*)il2cpp_codegen_static_fields_for(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var))->___InsecureInstance_0;
		return L_0;
	}
}
// Grpc.Core.ChannelCredentials Grpc.Core.ChannelCredentials::get_SecureSsl()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ChannelCredentials_get_SecureSsl_mD196C0EF0F502A90482AFA258AC96DC385DBB69C (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		il2cpp_codegen_runtime_class_init_inline(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_0 = ((ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_StaticFields*)il2cpp_codegen_static_fields_for(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var))->___SecureSslInstance_1;
		return L_0;
	}
}
// Grpc.Core.ChannelCredentials Grpc.Core.ChannelCredentials::Create(Grpc.Core.ChannelCredentials,Grpc.Core.CallCredentials)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ChannelCredentials_Create_m52838EDFB18FDC71585C4681986EE9AFB56BB2E9 (ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ___0_channelCredentials, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___1_callCredentials, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_0 = ___0_channelCredentials;
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_1 = ___1_callCredentials;
		CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740* L_2 = (CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740*)il2cpp_codegen_object_new(CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		CompositeChannelCredentials__ctor_mECB83F30B1E805ACD5F037105A1C793077ED0E86(L_2, L_0, L_1, NULL);
		return L_2;
	}
}
// System.Boolean Grpc.Core.ChannelCredentials::get_IsComposable()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool ChannelCredentials_get_IsComposable_mBD9F3E0EA5A668B7B9D525E722BA8D38B5490DC4 (ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* __this, const RuntimeMethod* method) 
{
	{
		return (bool)0;
	}
}
// System.Void Grpc.Core.ChannelCredentials::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ChannelCredentials__cctor_m542790D85B1CB9D2C153087809B46ED5C0A3E9CE (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A* L_0 = (InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A*)il2cpp_codegen_object_new(InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		InsecureCredentials__ctor_mBCDF914314FAAF1F79AD2D0F41C301CC4E315C12(L_0, NULL);
		((ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_StaticFields*)il2cpp_codegen_static_fields_for(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var))->___InsecureInstance_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&((ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_StaticFields*)il2cpp_codegen_static_fields_for(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var))->___InsecureInstance_0), (void*)L_0);
		SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* L_1 = (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3*)il2cpp_codegen_object_new(SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		SslCredentials__ctor_mB827017C7E77D62BBEACEFE1C7455AB9D619DFB3(L_1, NULL);
		((ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_StaticFields*)il2cpp_codegen_static_fields_for(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var))->___SecureSslInstance_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&((ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_StaticFields*)il2cpp_codegen_static_fields_for(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var))->___SecureSslInstance_1), (void*)L_1);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ChannelCredentials/InsecureCredentials::InternalPopulateConfiguration(Grpc.Core.ChannelCredentialsConfiguratorBase,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void InsecureCredentials_InternalPopulateConfiguration_mE50B8A435E7AABB344FD30EACA548EC10952CE40 (InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A* __this, ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E* ___0_configurator, RuntimeObject* ___1_state, const RuntimeMethod* method) 
{
	{
		ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E* L_0 = ___0_configurator;
		RuntimeObject* L_1 = ___1_state;
		NullCheck(L_0);
		VirtualActionInvoker1< RuntimeObject* >::Invoke(4 /* System.Void Grpc.Core.ChannelCredentialsConfiguratorBase::SetInsecureCredentials(System.Object) */, L_0, L_1);
		return;
	}
}
// System.Void Grpc.Core.ChannelCredentials/InsecureCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void InsecureCredentials__ctor_mBCDF914314FAAF1F79AD2D0F41C301CC4E315C12 (InsecureCredentials_tC5E457733E199F0D799AA3144789D31B3026DB6A* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		il2cpp_codegen_runtime_class_init_inline(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		ChannelCredentials__ctor_m5BAB665085468AD860D966A9B09538F491150849(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ChannelCredentials/CompositeChannelCredentials::.ctor(Grpc.Core.ChannelCredentials,Grpc.Core.CallCredentials)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CompositeChannelCredentials__ctor_mECB83F30B1E805ACD5F037105A1C793077ED0E86 (CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740* __this, ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* ___0_channelCredentials, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* ___1_callCredentials, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisCallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466_mB0D2D758B2EE1819303789F98C090EA881FE012E_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_m13ACE0719712FCF60B60D0CFFBE35BAC3F9E1DB4_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		il2cpp_codegen_runtime_class_init_inline(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		ChannelCredentials__ctor_m5BAB665085468AD860D966A9B09538F491150849(__this, NULL);
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_0 = ___0_channelCredentials;
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_m13ACE0719712FCF60B60D0CFFBE35BAC3F9E1DB4(L_0, GrpcPreconditions_CheckNotNull_TisChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_m13ACE0719712FCF60B60D0CFFBE35BAC3F9E1DB4_RuntimeMethod_var);
		__this->___channelCredentials_2 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___channelCredentials_2), (void*)L_1);
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_2 = ___1_callCredentials;
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_3;
		L_3 = GrpcPreconditions_CheckNotNull_TisCallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466_mB0D2D758B2EE1819303789F98C090EA881FE012E(L_2, GrpcPreconditions_CheckNotNull_TisCallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466_mB0D2D758B2EE1819303789F98C090EA881FE012E_RuntimeMethod_var);
		__this->___callCredentials_3 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___callCredentials_3), (void*)L_3);
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_4 = ___0_channelCredentials;
		NullCheck(L_4);
		bool L_5;
		L_5 = VirtualFuncInvoker0< bool >::Invoke(5 /* System.Boolean Grpc.Core.ChannelCredentials::get_IsComposable() */, L_4);
		if (L_5)
		{
			goto IL_0041;
		}
	}
	{
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_6 = ___0_channelCredentials;
		NullCheck(L_6);
		Type_t* L_7;
		L_7 = Object_GetType_mE10A8FC1E57F3DF29972CCBC026C2DC3942263B3(L_6, NULL);
		NullCheck(L_7);
		String_t* L_8;
		L_8 = VirtualFuncInvoker0< String_t* >::Invoke(8 /* System.String System.Reflection.MemberInfo::get_Name() */, L_7);
		String_t* L_9;
		L_9 = String_Format_mA8DBB4C2516B9723C5A41E6CB1E2FAF4BBE96DD8(((String_t*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&_stringLiteral02E811106795854EAA599A5506DD0042594581C0)), L_8, NULL);
		ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263* L_10 = (ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263_il2cpp_TypeInfo_var)));
		NullCheck(L_10);
		ArgumentException__ctor_m026938A67AF9D36BB7ED27F80425D7194B514465(L_10, L_9, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_10, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&CompositeChannelCredentials__ctor_mECB83F30B1E805ACD5F037105A1C793077ED0E86_RuntimeMethod_var)));
	}

IL_0041:
	{
		return;
	}
}
// System.Void Grpc.Core.ChannelCredentials/CompositeChannelCredentials::InternalPopulateConfiguration(Grpc.Core.ChannelCredentialsConfiguratorBase,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void CompositeChannelCredentials_InternalPopulateConfiguration_m7198A9B3927DE7F45EE1C989CACCAD564C94B3DD (CompositeChannelCredentials_t268BBE6D2D23AA4AACFB836250510069001B2740* __this, ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E* ___0_configurator, RuntimeObject* ___1_state, const RuntimeMethod* method) 
{
	{
		ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E* L_0 = ___0_configurator;
		RuntimeObject* L_1 = ___1_state;
		ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730* L_2 = __this->___channelCredentials_2;
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_3 = __this->___callCredentials_3;
		NullCheck(L_0);
		VirtualActionInvoker3< RuntimeObject*, ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730*, CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* >::Invoke(6 /* System.Void Grpc.Core.ChannelCredentialsConfiguratorBase::SetCompositeCredentials(System.Object,Grpc.Core.ChannelCredentials,Grpc.Core.CallCredentials) */, L_0, L_1, L_2, L_3);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ChannelCredentialsConfiguratorBase::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ChannelCredentialsConfiguratorBase__ctor_m16E0CB00D0F46E6BD053F90FAA5306469E11C065 (ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ClientBase::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBase__ctor_mEB0C996C742BE0DA0352B0CB222312E57836FDDB (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435* L_0 = (UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435*)il2cpp_codegen_object_new(UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		UnimplementedCallInvoker__ctor_m56CA5FABA47729295B56022F92B907D32DBFA0AD(L_0, NULL);
		ClientBase__ctor_mBB9AD81C869A8006A8C422E7AD83AE0ADBD573DB(__this, L_0, NULL);
		return;
	}
}
// System.Void Grpc.Core.ClientBase::.ctor(Grpc.Core.ClientBase/ClientBaseConfiguration)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBase__ctor_mA7F300DBD8471A62FCD6475413DFF3B7783B6D70 (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* ___0_configuration, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_m7EB6EB36D0C253043670DC0A19000775E31CB16C_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralE8BC822461C381F0D1DE9430568E970322E2D07A);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* L_0 = ___0_configuration;
		ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_m7EB6EB36D0C253043670DC0A19000775E31CB16C(L_0, _stringLiteralE8BC822461C381F0D1DE9430568E970322E2D07A, GrpcPreconditions_CheckNotNull_TisClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_m7EB6EB36D0C253043670DC0A19000775E31CB16C_RuntimeMethod_var);
		__this->___configuration_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___configuration_0), (void*)L_1);
		ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* L_2 = ___0_configuration;
		NullCheck(L_2);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_3;
		L_3 = ClientBaseConfiguration_CreateDecoratedCallInvoker_mE305C7894FB598F137DC120396393B2AA6A5E7B6(L_2, NULL);
		__this->___callInvoker_1 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___callInvoker_1), (void*)L_3);
		return;
	}
}
// System.Void Grpc.Core.ClientBase::.ctor(Grpc.Core.ChannelBase)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBase__ctor_m004331C7DEC80A3EF71FF7FD553CA8A110720370 (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* ___0_channel, const RuntimeMethod* method) 
{
	{
		ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* L_0 = ___0_channel;
		NullCheck(L_0);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_1;
		L_1 = VirtualFuncInvoker0< CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* >::Invoke(4 /* Grpc.Core.CallInvoker Grpc.Core.ChannelBase::CreateCallInvoker() */, L_0);
		ClientBase__ctor_mBB9AD81C869A8006A8C422E7AD83AE0ADBD573DB(__this, L_1, NULL);
		return;
	}
}
// System.Void Grpc.Core.ClientBase::.ctor(Grpc.Core.CallInvoker)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBase__ctor_mBB9AD81C869A8006A8C422E7AD83AE0ADBD573DB (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_callInvoker, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = ___0_callInvoker;
		ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* L_1 = (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D*)il2cpp_codegen_object_new(ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		ClientBaseConfiguration__ctor_m3BA0C26F2E8F95872BCDDCEAAD302B98CD4BE33F(L_1, L_0, (String_t*)NULL, NULL);
		ClientBase__ctor_mA7F300DBD8471A62FCD6475413DFF3B7783B6D70(__this, L_1, NULL);
		return;
	}
}
// Grpc.Core.CallInvoker Grpc.Core.ClientBase::get_CallInvoker()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ClientBase_get_CallInvoker_mF3E51D6FE5AA4B3A732F085DEDE995249ECF4788 (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, const RuntimeMethod* method) 
{
	{
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = __this->___callInvoker_1;
		return L_0;
	}
}
// Grpc.Core.ClientBase/ClientBaseConfiguration Grpc.Core.ClientBase::get_Configuration()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* ClientBase_get_Configuration_mED3A66968C03DFE65340A9CF422F64362DCE55F1 (ClientBase_tD3AE7DEE97761650B2A6CCB19E3B501FEF713391* __this, const RuntimeMethod* method) 
{
	{
		ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* L_0 = __this->___configuration_0;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ClientBase/ClientBaseConfiguration::.ctor(Grpc.Core.CallInvoker,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBaseConfiguration__ctor_m3BA0C26F2E8F95872BCDDCEAAD302B98CD4BE33F (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* __this, CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_undecoratedCallInvoker, String_t* ___1_host, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_mAD4DE615EDB070D206521A1845F088B4ABAC8966_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = ___0_undecoratedCallInvoker;
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_mAD4DE615EDB070D206521A1845F088B4ABAC8966(L_0, GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_mAD4DE615EDB070D206521A1845F088B4ABAC8966_RuntimeMethod_var);
		__this->___undecoratedCallInvoker_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___undecoratedCallInvoker_0), (void*)L_1);
		String_t* L_2 = ___1_host;
		__this->___host_1 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___host_1), (void*)L_2);
		return;
	}
}
// Grpc.Core.CallInvoker Grpc.Core.ClientBase/ClientBaseConfiguration::CreateDecoratedCallInvoker()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ClientBaseConfiguration_CreateDecoratedCallInvoker_mE305C7894FB598F137DC120396393B2AA6A5E7B6 (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ClientBaseConfiguration_U3CCreateDecoratedCallInvokerU3Eb__5_0_m6D33050551D69C47894B758AB866E63FDE9F2FD8_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = __this->___undecoratedCallInvoker_0;
		Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* L_1 = (Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20*)il2cpp_codegen_object_new(Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		Func_4__ctor_mC4ADA479232073776D307BC5CDB7E2B13E82A910(L_1, __this, (intptr_t)((void*)ClientBaseConfiguration_U3CCreateDecoratedCallInvokerU3Eb__5_0_m6D33050551D69C47894B758AB866E63FDE9F2FD8_RuntimeMethod_var), NULL);
		ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425* L_2 = (ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425*)il2cpp_codegen_object_new(ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		ClientBaseConfigurationInterceptor__ctor_m0DA8B4A915C09AACF2B1C2EB819505EC59A06583(L_2, L_1, NULL);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_3;
		L_3 = CallInvokerExtensions_Intercept_m415B574E557536E895A7DAC8AE5B3B14026B135A(L_0, L_2, NULL);
		return L_3;
	}
}
// Grpc.Core.ClientBase/ClientBaseConfiguration Grpc.Core.ClientBase/ClientBaseConfiguration::WithHost(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* ClientBaseConfiguration_WithHost_mE8C64EB57055550D95290E2E1B5C1C5FF703E536 (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* __this, String_t* ___0_host, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralE705C6345C26AF82E64D22DBE44B2A3514F2F06F);
		s_Il2CppMethodInitialized = true;
	}
	{
		String_t* L_0 = ___0_host;
		String_t* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B(L_0, _stringLiteralE705C6345C26AF82E64D22DBE44B2A3514F2F06F, GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_2 = __this->___undecoratedCallInvoker_0;
		String_t* L_3 = ___0_host;
		ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* L_4 = (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D*)il2cpp_codegen_object_new(ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D_il2cpp_TypeInfo_var);
		NullCheck(L_4);
		ClientBaseConfiguration__ctor_m3BA0C26F2E8F95872BCDDCEAAD302B98CD4BE33F(L_4, L_2, L_3, NULL);
		return L_4;
	}
}
// Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo Grpc.Core.ClientBase/ClientBaseConfiguration::<CreateDecoratedCallInvoker>b__5_0(Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9 ClientBaseConfiguration_U3CCreateDecoratedCallInvokerU3Eb__5_0_m6D33050551D69C47894B758AB866E63FDE9F2FD8 (ClientBaseConfiguration_t6683C515CEA1065CA71E4BC27396CA44DBA8F46D* __this, RuntimeObject* ___0_method, String_t* ___1_host, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E ___2_options, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___host_1;
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_1 = ___2_options;
		ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9 L_2;
		memset((&L_2), 0, sizeof(L_2));
		ClientBaseConfigurationInfo__ctor_m7942448EB246ED72C3F9B3D64896727E92D3C679((&L_2), L_0, L_1, /*hidden argument*/NULL);
		return L_2;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInterceptor::.ctor(System.Func`4<Grpc.Core.IMethod,System.String,Grpc.Core.CallOptions,Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBaseConfigurationInterceptor__ctor_m0DA8B4A915C09AACF2B1C2EB819505EC59A06583 (ClientBaseConfigurationInterceptor_t42B99FC61611024704403A5CB0436F1E12776425* __this, Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* ___0_interceptor, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisFunc_4_t737C862B437880FF1CD553B949328C8BCF76DD20_m5CD271FF760C0D77688A24811BE9FF6518D3090D_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral29D01ED29B3E08D159739A60EE14FFC25AC4F8FE);
		s_Il2CppMethodInitialized = true;
	}
	{
		Interceptor__ctor_m0A48ED17B15161D356A95F546D1AADB995725169(__this, NULL);
		Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* L_0 = ___0_interceptor;
		Func_4_t737C862B437880FF1CD553B949328C8BCF76DD20* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisFunc_4_t737C862B437880FF1CD553B949328C8BCF76DD20_m5CD271FF760C0D77688A24811BE9FF6518D3090D(L_0, _stringLiteral29D01ED29B3E08D159739A60EE14FFC25AC4F8FE, GrpcPreconditions_CheckNotNull_TisFunc_4_t737C862B437880FF1CD553B949328C8BCF76DD20_m5CD271FF760C0D77688A24811BE9FF6518D3090D_RuntimeMethod_var);
		__this->___interceptor_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___interceptor_0), (void*)L_1);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif


// Conversion methods for marshalling of: Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo
IL2CPP_EXTERN_C void ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshal_pinvoke(const ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9& unmarshaled, ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_pinvoke& marshaled)
{
	Exception_t* ___CallOptions_1Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'CallOptions' of type 'ClientBaseConfigurationInfo'.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___CallOptions_1Exception, NULL);
}
IL2CPP_EXTERN_C void ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshal_pinvoke_back(const ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_pinvoke& marshaled, ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9& unmarshaled)
{
	Exception_t* ___CallOptions_1Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'CallOptions' of type 'ClientBaseConfigurationInfo'.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___CallOptions_1Exception, NULL);
}
// Conversion method for clean up from marshalling of: Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo
IL2CPP_EXTERN_C void ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshal_pinvoke_cleanup(ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_pinvoke& marshaled)
{
}


// Conversion methods for marshalling of: Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo
IL2CPP_EXTERN_C void ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshal_com(const ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9& unmarshaled, ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_com& marshaled)
{
	Exception_t* ___CallOptions_1Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'CallOptions' of type 'ClientBaseConfigurationInfo'.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___CallOptions_1Exception, NULL);
}
IL2CPP_EXTERN_C void ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshal_com_back(const ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_com& marshaled, ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9& unmarshaled)
{
	Exception_t* ___CallOptions_1Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field 'CallOptions' of type 'ClientBaseConfigurationInfo'.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___CallOptions_1Exception, NULL);
}
// Conversion method for clean up from marshalling of: Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo
IL2CPP_EXTERN_C void ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshal_com_cleanup(ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9_marshaled_com& marshaled)
{
}
// System.Void Grpc.Core.ClientBase/ClientBaseConfiguration/ClientBaseConfigurationInfo::.ctor(System.String,Grpc.Core.CallOptions)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ClientBaseConfigurationInfo__ctor_m7942448EB246ED72C3F9B3D64896727E92D3C679 (ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9* __this, String_t* ___0_host, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E ___1_callOptions, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = ___0_host;
		__this->___Host_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___Host_0), (void*)L_0);
		CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E L_1 = ___1_callOptions;
		__this->___CallOptions_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)&(((&__this->___CallOptions_1))->___headers_0), (void*)NULL);
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&((&(((&__this->___CallOptions_1))->___cancellationToken_2))->____source_0), (void*)NULL);
		#endif
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&(((&__this->___CallOptions_1))->___writeOptions_3), (void*)NULL);
		#endif
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&(((&__this->___CallOptions_1))->___propagationToken_4), (void*)NULL);
		#endif
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&(((&__this->___CallOptions_1))->___credentials_5), (void*)NULL);
		#endif
		return;
	}
}
IL2CPP_EXTERN_C  void ClientBaseConfigurationInfo__ctor_m7942448EB246ED72C3F9B3D64896727E92D3C679_AdjustorThunk (RuntimeObject* __this, String_t* ___0_host, CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E ___1_callOptions, const RuntimeMethod* method)
{
	ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<ClientBaseConfigurationInfo_tC073D631DC48CCC2AE6820C9D269A648992C23F9*>(__this + _offset);
	ClientBaseConfigurationInfo__ctor_m7942448EB246ED72C3F9B3D64896727E92D3C679(_thisAdjusted, ___0_host, ___1_callOptions, method);
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ContextPropagationOptions::.ctor(System.Boolean,System.Boolean)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ContextPropagationOptions__ctor_m96E390576401E7BA84886C7E6F3BDD38CA9C3E4A (ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* __this, bool ___0_propagateDeadline, bool ___1_propagateCancellation, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		bool L_0 = ___0_propagateDeadline;
		__this->___propagateDeadline_1 = L_0;
		bool L_1 = ___1_propagateCancellation;
		__this->___propagateCancellation_2 = L_1;
		return;
	}
}
// System.Boolean Grpc.Core.ContextPropagationOptions::get_IsPropagateDeadline()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool ContextPropagationOptions_get_IsPropagateDeadline_mA256BB3575ABCB99D82FAC8EEF4E504BFE239457 (ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* __this, const RuntimeMethod* method) 
{
	{
		bool L_0 = __this->___propagateDeadline_1;
		return L_0;
	}
}
// System.Boolean Grpc.Core.ContextPropagationOptions::get_IsPropagateCancellation()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool ContextPropagationOptions_get_IsPropagateCancellation_m82C91F6179064F818334226C75E07FFDE2EBCE48 (ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* __this, const RuntimeMethod* method) 
{
	{
		bool L_0 = __this->___propagateCancellation_2;
		return L_0;
	}
}
// System.Void Grpc.Core.ContextPropagationOptions::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ContextPropagationOptions__cctor_mF445F65E29D02AC6409030BAA9A783B6B5415411 (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* L_0 = (ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B*)il2cpp_codegen_object_new(ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		ContextPropagationOptions__ctor_m96E390576401E7BA84886C7E6F3BDD38CA9C3E4A(L_0, (bool)1, (bool)1, NULL);
		((ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_StaticFields*)il2cpp_codegen_static_fields_for(ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_il2cpp_TypeInfo_var))->___Default_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&((ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_StaticFields*)il2cpp_codegen_static_fields_for(ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B_il2cpp_TypeInfo_var))->___Default_0), (void*)L_0);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ContextPropagationToken::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ContextPropagationToken__ctor_mCD270D0418969C7264695ED0678CD9BE9162895A (ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Byte[] Grpc.Core.DeserializationContext::PayloadAsNewBuffer()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* DeserializationContext_PayloadAsNewBuffer_m90E78A24BD2E0B2AB24095E4C405D6610D51D6D0 (DeserializationContext_t8C084AFEE582F438C6A9F8392DB3C4A92A930FAC* __this, const RuntimeMethod* method) 
{
	{
		NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8* L_0 = (NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8_il2cpp_TypeInfo_var)));
		NullCheck(L_0);
		NotImplementedException__ctor_mDAB47BC6BD0E342E8F2171E5CABE3E67EA049F1C(L_0, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_0, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&DeserializationContext_PayloadAsNewBuffer_m90E78A24BD2E0B2AB24095E4C405D6610D51D6D0_RuntimeMethod_var)));
	}
}
// System.Buffers.ReadOnlySequence`1<System.Byte> Grpc.Core.DeserializationContext::PayloadAsReadOnlySequence()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ReadOnlySequence_1_t85097489C00952FC58DF9F3ACE926EAE9A51898A DeserializationContext_PayloadAsReadOnlySequence_m18B455D5E3F552FA7055CDD31AD32151DCFB1441 (DeserializationContext_t8C084AFEE582F438C6A9F8392DB3C4A92A930FAC* __this, const RuntimeMethod* method) 
{
	{
		NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8* L_0 = (NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8_il2cpp_TypeInfo_var)));
		NullCheck(L_0);
		NotImplementedException__ctor_mDAB47BC6BD0E342E8F2171E5CABE3E67EA049F1C(L_0, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_0, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&DeserializationContext_PayloadAsReadOnlySequence_m18B455D5E3F552FA7055CDD31AD32151DCFB1441_RuntimeMethod_var)));
	}
}
// System.Void Grpc.Core.DeserializationContext::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void DeserializationContext__ctor_m1E6D1666DB5CCCA05EFC79B75CD1322443FC136E (DeserializationContext_t8C084AFEE582F438C6A9F8392DB3C4A92A930FAC* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.KeyCertificatePair::.ctor(System.String,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void KeyCertificatePair__ctor_m287B891FFB945275BA6945E702E44298BAAB8F62 (KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* __this, String_t* ___0_certificateChain, String_t* ___1_privateKey, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral0D292B6278EB6FF9C4EA2C48A1C5CE6FB09E1678);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral2EC6DE2D71C83B236D586235EA0A7A8E7FF32E9F);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_certificateChain;
		String_t* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B(L_0, _stringLiteral0D292B6278EB6FF9C4EA2C48A1C5CE6FB09E1678, GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		__this->___certificateChain_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___certificateChain_0), (void*)L_1);
		String_t* L_2 = ___1_privateKey;
		String_t* L_3;
		L_3 = GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B(L_2, _stringLiteral2EC6DE2D71C83B236D586235EA0A7A8E7FF32E9F, GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		__this->___privateKey_1 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___privateKey_1), (void*)L_3);
		return;
	}
}
// System.String Grpc.Core.KeyCertificatePair::get_CertificateChain()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* KeyCertificatePair_get_CertificateChain_m9766039E4F73077B3AEEF95F2F9CC64E9D39247F (KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___certificateChain_0;
		return L_0;
	}
}
// System.String Grpc.Core.KeyCertificatePair::get_PrivateKey()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* KeyCertificatePair_get_PrivateKey_m9C3E9820368C49C0497B81A1591A25292C56681A (KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___privateKey_1;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// Grpc.Core.Marshaller`1<System.String> Grpc.Core.Marshallers::get_StringMarshaller()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04* Marshallers_get_StringMarshaller_m713DFDD62092F62355BF9F037C7FD7B84A5A4D9F (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Marshaller_1__ctor_mF20CB78867ABA38F189F7DCE7D6DCA50C197A6DE_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_0;
		L_0 = Encoding_get_UTF8_m9FA98A53CE96FD6D02982625C5246DD36C1235C9(NULL);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_1 = L_0;
		Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C* L_2 = (Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C*)il2cpp_codegen_object_new(Func_2_tFEA0423DEC967218958370EA6128F055D0901D9C_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		Func_2__ctor_m573F485A6595759C50F07B6051007D317FE3E27F(L_2, L_1, (intptr_t)((void*)GetVirtualMethodInfo(L_1, 17)), NULL);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_3;
		L_3 = Encoding_get_UTF8_m9FA98A53CE96FD6D02982625C5246DD36C1235C9(NULL);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_4 = L_3;
		Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF* L_5 = (Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF*)il2cpp_codegen_object_new(Func_2_tFBD229E709A43205BA004079BD6829CFEDAC6FFF_il2cpp_TypeInfo_var);
		NullCheck(L_5);
		Func_2__ctor_m0779F5BE8A3C3178996AC0BE54575D71FB4D35B4(L_5, L_4, (intptr_t)((void*)GetVirtualMethodInfo(L_4, 34)), NULL);
		Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04* L_6 = (Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04*)il2cpp_codegen_object_new(Marshaller_1_t4E74B023A71CCEBCD2CB766291F697A831A0FB04_il2cpp_TypeInfo_var);
		NullCheck(L_6);
		Marshaller_1__ctor_mF20CB78867ABA38F189F7DCE7D6DCA50C197A6DE(L_6, L_2, L_5, Marshaller_1__ctor_mF20CB78867ABA38F189F7DCE7D6DCA50C197A6DE_RuntimeMethod_var);
		return L_6;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Metadata::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata__ctor_mAA8F9E779B141794B1A58388BE41D32F97B0085A (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1__ctor_mC61673FF37950228AD28C7E2D5798F7E822165FE_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = (List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337*)il2cpp_codegen_object_new(List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		List_1__ctor_mC61673FF37950228AD28C7E2D5798F7E822165FE(L_0, List_1__ctor_mC61673FF37950228AD28C7E2D5798F7E822165FE_RuntimeMethod_var);
		__this->___entries_4 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___entries_4), (void*)L_0);
		return;
	}
}
// Grpc.Core.Metadata Grpc.Core.Metadata::Freeze()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* Metadata_Freeze_m6A755BCE0A04B4A1B690D05277D90BBD1F3A4AD1 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	{
		__this->___readOnly_5 = (bool)1;
		return __this;
	}
}
// Grpc.Core.Metadata/Entry Grpc.Core.Metadata::Get(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* Metadata_Get_mAB88DB01CE0B1319FB4E239A795DFD6B1CF6F506 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, String_t* ___0_key, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	int32_t V_0 = 0;
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		NullCheck(L_0);
		int32_t L_1;
		L_1 = List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_inline(L_0, List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_RuntimeMethod_var);
		V_0 = ((int32_t)il2cpp_codegen_subtract(L_1, 1));
		goto IL_0035;
	}

IL_0010:
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_2 = __this->___entries_4;
		int32_t L_3 = V_0;
		NullCheck(L_2);
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_4;
		L_4 = List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9(L_2, L_3, List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		String_t* L_5 = ___0_key;
		NullCheck(L_4);
		bool L_6;
		L_6 = Entry_KeyEqualsIgnoreCase_m5765029234BADCFDF40F5E1BD2E4CF3313D10A27(L_4, L_5, NULL);
		if (!L_6)
		{
			goto IL_0031;
		}
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_7 = __this->___entries_4;
		int32_t L_8 = V_0;
		NullCheck(L_7);
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_9;
		L_9 = List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9(L_7, L_8, List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		return L_9;
	}

IL_0031:
	{
		int32_t L_10 = V_0;
		V_0 = ((int32_t)il2cpp_codegen_subtract(L_10, 1));
	}

IL_0035:
	{
		int32_t L_11 = V_0;
		if ((((int32_t)L_11) >= ((int32_t)0)))
		{
			goto IL_0010;
		}
	}
	{
		return (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*)NULL;
	}
}
// System.String Grpc.Core.Metadata::GetValue(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Metadata_GetValue_m7FF6849B2A8414BB895CA9E64F65A12E8EFF5559 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, String_t* ___0_key, const RuntimeMethod* method) 
{
	Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* G_B2_0 = NULL;
	Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* G_B1_0 = NULL;
	{
		String_t* L_0 = ___0_key;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1;
		L_1 = Metadata_Get_mAB88DB01CE0B1319FB4E239A795DFD6B1CF6F506(__this, L_0, NULL);
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_2 = L_1;
		G_B1_0 = L_2;
		if (L_2)
		{
			G_B2_0 = L_2;
			goto IL_000d;
		}
	}
	{
		return (String_t*)NULL;
	}

IL_000d:
	{
		NullCheck(G_B2_0);
		String_t* L_3;
		L_3 = Entry_get_Value_m1C0D0600AFC16B36093ED5B021335B952C4FD696(G_B2_0, NULL);
		return L_3;
	}
}
// System.Byte[] Grpc.Core.Metadata::GetValueBytes(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* Metadata_GetValueBytes_m9C17CBC1626218437117573B9C452169E82FD066 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, String_t* ___0_key, const RuntimeMethod* method) 
{
	Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* G_B2_0 = NULL;
	Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* G_B1_0 = NULL;
	{
		String_t* L_0 = ___0_key;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1;
		L_1 = Metadata_Get_mAB88DB01CE0B1319FB4E239A795DFD6B1CF6F506(__this, L_0, NULL);
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_2 = L_1;
		G_B1_0 = L_2;
		if (L_2)
		{
			G_B2_0 = L_2;
			goto IL_000d;
		}
	}
	{
		return (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)NULL;
	}

IL_000d:
	{
		NullCheck(G_B2_0);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_3;
		L_3 = Entry_get_ValueBytes_m8952343C41FE19A9A5190F2C15D71BDB3CF8F5B9(G_B2_0, NULL);
		return L_3;
	}
}
// System.Collections.Generic.IEnumerable`1<Grpc.Core.Metadata/Entry> Grpc.Core.Metadata::GetAll(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* Metadata_GetAll_m55217D660937AA054ED43A48A1F02878588FFC74 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, String_t* ___0_key, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* L_0 = (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3*)il2cpp_codegen_object_new(U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		U3CGetAllU3Ed__11__ctor_m35967E7C5537A498542DC7F352BD828EA734D4A0(L_0, ((int32_t)-2), NULL);
		U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* L_1 = L_0;
		NullCheck(L_1);
		L_1->___U3CU3E4__this_3 = __this;
		Il2CppCodeGenWriteBarrier((void**)(&L_1->___U3CU3E4__this_3), (void*)__this);
		U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* L_2 = L_1;
		String_t* L_3 = ___0_key;
		NullCheck(L_2);
		L_2->___U3CU3E3__key_5 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&L_2->___U3CU3E3__key_5), (void*)L_3);
		return L_2;
	}
}
// System.Void Grpc.Core.Metadata::Add(System.String,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_Add_m1A1A01CC7BB5C7428BC425672ABF435B49CC414C (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, String_t* ___0_key, String_t* ___1_value, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		String_t* L_0 = ___0_key;
		String_t* L_1 = ___1_value;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_2 = (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*)il2cpp_codegen_object_new(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		Entry__ctor_m06E649C4EA82D12900A074292784F649C3D0538D(L_2, L_0, L_1, NULL);
		Metadata_Add_m189C162DF2490839465923F96C443BA1189D8B2A(__this, L_2, NULL);
		return;
	}
}
// System.Void Grpc.Core.Metadata::Add(System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_Add_m088187AB3A23C541CB729A210B12E9770439B828 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, String_t* ___0_key, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_valueBytes, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		String_t* L_0 = ___0_key;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_1 = ___1_valueBytes;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_2 = (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*)il2cpp_codegen_object_new(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		Entry__ctor_m7F675669248FB0A305E5B705BEC9550446AF5B0A(L_2, L_0, L_1, NULL);
		Metadata_Add_m189C162DF2490839465923F96C443BA1189D8B2A(__this, L_2, NULL);
		return;
	}
}
// System.Int32 Grpc.Core.Metadata::IndexOf(Grpc.Core.Metadata/Entry)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t Metadata_IndexOf_m5E31D48C1FA2A23D2AE0A41BDCEF3B31D56AC24F (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_IndexOf_m098D47D60BAF4C30BB0E8FF3CF8EF49093D6FD2B_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1 = ___0_item;
		NullCheck(L_0);
		int32_t L_2;
		L_2 = List_1_IndexOf_m098D47D60BAF4C30BB0E8FF3CF8EF49093D6FD2B(L_0, L_1, List_1_IndexOf_m098D47D60BAF4C30BB0E8FF3CF8EF49093D6FD2B_RuntimeMethod_var);
		return L_2;
	}
}
// System.Void Grpc.Core.Metadata::Insert(System.Int32,Grpc.Core.Metadata/Entry)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_Insert_m8B5EE7F67DB8C66EE7EEA07AF1E4456194505C2C (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, int32_t ___0_index, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___1_item, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_Insert_m383E4FA9E7D234C2BFAC3B41E038B15CFA80AF05_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_0 = ___1_item;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E(L_0, GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E_RuntimeMethod_var);
		Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4(__this, NULL);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_2 = __this->___entries_4;
		int32_t L_3 = ___0_index;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_4 = ___1_item;
		NullCheck(L_2);
		List_1_Insert_m383E4FA9E7D234C2BFAC3B41E038B15CFA80AF05(L_2, L_3, L_4, List_1_Insert_m383E4FA9E7D234C2BFAC3B41E038B15CFA80AF05_RuntimeMethod_var);
		return;
	}
}
// System.Void Grpc.Core.Metadata::RemoveAt(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_RemoveAt_m6D896E1498EF357DEAA116B6FE334764B79B903F (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, int32_t ___0_index, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_RemoveAt_m76954D81D216842D8A3ADD420E6588EABFA34A6A_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4(__this, NULL);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		int32_t L_1 = ___0_index;
		NullCheck(L_0);
		List_1_RemoveAt_m76954D81D216842D8A3ADD420E6588EABFA34A6A(L_0, L_1, List_1_RemoveAt_m76954D81D216842D8A3ADD420E6588EABFA34A6A_RuntimeMethod_var);
		return;
	}
}
// Grpc.Core.Metadata/Entry Grpc.Core.Metadata::get_Item(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* Metadata_get_Item_m164FD39FB720D5F360041A9EA54923AE5295E224 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, int32_t ___0_index, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		int32_t L_1 = ___0_index;
		NullCheck(L_0);
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_2;
		L_2 = List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9(L_0, L_1, List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		return L_2;
	}
}
// System.Void Grpc.Core.Metadata::set_Item(System.Int32,Grpc.Core.Metadata/Entry)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_set_Item_m3E1C1837D01FF27719AE7C2C0DACDB6358C4A2A0 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, int32_t ___0_index, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___1_value, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_set_Item_mDBC1AAC216D70CE8B5E8CB3AAF3083E65247CFC3_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_0 = ___1_value;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E(L_0, GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E_RuntimeMethod_var);
		Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4(__this, NULL);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_2 = __this->___entries_4;
		int32_t L_3 = ___0_index;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_4 = ___1_value;
		NullCheck(L_2);
		List_1_set_Item_mDBC1AAC216D70CE8B5E8CB3AAF3083E65247CFC3(L_2, L_3, L_4, List_1_set_Item_mDBC1AAC216D70CE8B5E8CB3AAF3083E65247CFC3_RuntimeMethod_var);
		return;
	}
}
// System.Void Grpc.Core.Metadata::Add(Grpc.Core.Metadata/Entry)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_Add_m189C162DF2490839465923F96C443BA1189D8B2A (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_Add_m8626F0812751DA3B7FF22FC178565F1326A68CD0_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_0 = ___0_item;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E(L_0, GrpcPreconditions_CheckNotNull_TisEntry_tD4A927251AFDB94306DBF05F34FE7346944555BB_m865C1FF052D253E9EDE7CF942C09295E5DB7077E_RuntimeMethod_var);
		Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4(__this, NULL);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_2 = __this->___entries_4;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_3 = ___0_item;
		NullCheck(L_2);
		List_1_Add_m8626F0812751DA3B7FF22FC178565F1326A68CD0_inline(L_2, L_3, List_1_Add_m8626F0812751DA3B7FF22FC178565F1326A68CD0_RuntimeMethod_var);
		return;
	}
}
// System.Void Grpc.Core.Metadata::Clear()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_Clear_mA2CB7DF5AD1A3806DD69FDB256210BA3F7F213A4 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_Clear_mAAD225470B9D453E533818038DA07BC1494513DC_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4(__this, NULL);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		NullCheck(L_0);
		List_1_Clear_mAAD225470B9D453E533818038DA07BC1494513DC_inline(L_0, List_1_Clear_mAAD225470B9D453E533818038DA07BC1494513DC_RuntimeMethod_var);
		return;
	}
}
// System.Boolean Grpc.Core.Metadata::Contains(Grpc.Core.Metadata/Entry)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Metadata_Contains_m661986DE093F0DE5E477BD27EB0E48DE063AB2D6 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_Contains_m3750DD90BC38CC246D898710BA06F7F266DEFB78_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1 = ___0_item;
		NullCheck(L_0);
		bool L_2;
		L_2 = List_1_Contains_m3750DD90BC38CC246D898710BA06F7F266DEFB78(L_0, L_1, List_1_Contains_m3750DD90BC38CC246D898710BA06F7F266DEFB78_RuntimeMethod_var);
		return L_2;
	}
}
// System.Void Grpc.Core.Metadata::CopyTo(Grpc.Core.Metadata/Entry[],System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_CopyTo_mA132E8C70907A9657C8E7DB37DB29D9D54B2D4BF (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4* ___0_array, int32_t ___1_arrayIndex, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_CopyTo_m6CA081F6251F594F64C73BCB3B6915259FB42903_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		EntryU5BU5D_t982B8F3973A9261FEFF0C442E74E258C4C79F9E4* L_1 = ___0_array;
		int32_t L_2 = ___1_arrayIndex;
		NullCheck(L_0);
		List_1_CopyTo_m6CA081F6251F594F64C73BCB3B6915259FB42903(L_0, L_1, L_2, List_1_CopyTo_m6CA081F6251F594F64C73BCB3B6915259FB42903_RuntimeMethod_var);
		return;
	}
}
// System.Int32 Grpc.Core.Metadata::get_Count()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t Metadata_get_Count_m29EFA2761841A38903EAF2DCEBC5DB9B06BCDFB8 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		NullCheck(L_0);
		int32_t L_1;
		L_1 = List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_inline(L_0, List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_RuntimeMethod_var);
		return L_1;
	}
}
// System.Boolean Grpc.Core.Metadata::get_IsReadOnly()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Metadata_get_IsReadOnly_m441E6B1CFC8BBD8E8FE6BE2332ACC568D88D3886 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	{
		bool L_0 = __this->___readOnly_5;
		return L_0;
	}
}
// System.Boolean Grpc.Core.Metadata::Remove(Grpc.Core.Metadata/Entry)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Metadata_Remove_m5EFD4B4539CAFF0C112620C88C18AEEB7CC106F2 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* ___0_item, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_Remove_mBE506315950A1EA3F5351D3E7A9216015D68A6FA_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4(__this, NULL);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_1 = ___0_item;
		NullCheck(L_0);
		bool L_2;
		L_2 = List_1_Remove_mBE506315950A1EA3F5351D3E7A9216015D68A6FA(L_0, L_1, List_1_Remove_mBE506315950A1EA3F5351D3E7A9216015D68A6FA_RuntimeMethod_var);
		return L_2;
	}
}
// System.Collections.Generic.IEnumerator`1<Grpc.Core.Metadata/Entry> Grpc.Core.Metadata::GetEnumerator()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* Metadata_GetEnumerator_m212BFF5FCB9A1B3E93EA4E1CABD0B2DC4FC6CBE1 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		NullCheck(L_0);
		Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6 L_1;
		L_1 = List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101(L_0, List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101_RuntimeMethod_var);
		Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6 L_2 = L_1;
		RuntimeObject* L_3 = Box(Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6_il2cpp_TypeInfo_var, &L_2);
		return (RuntimeObject*)L_3;
	}
}
// System.Collections.IEnumerator Grpc.Core.Metadata::System.Collections.IEnumerable.GetEnumerator()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* Metadata_System_Collections_IEnumerable_GetEnumerator_mF919100B00A48CE43CCF3041474F15450EE6AB9F (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_0 = __this->___entries_4;
		NullCheck(L_0);
		Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6 L_1;
		L_1 = List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101(L_0, List_1_GetEnumerator_m538DD57E21C242DFC8CD3C4467D32B6669B8B101_RuntimeMethod_var);
		Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6 L_2 = L_1;
		RuntimeObject* L_3 = Box(Enumerator_tBE2644DDDCFD1D97DBEBCAC4E1883A9D2B1584D6_il2cpp_TypeInfo_var, &L_2);
		return (RuntimeObject*)L_3;
	}
}
// System.Void Grpc.Core.Metadata::CheckWriteable()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata_CheckWriteable_mF23787FA36135A552584B0745D5BA9EE82130BC4 (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral822EDE75F5970A676E660AB56E0A62A92F681766);
		s_Il2CppMethodInitialized = true;
	}
	{
		bool L_0 = __this->___readOnly_5;
		GrpcPreconditions_CheckState_mCDE04614CE69128AF99EE030B5F83D386FDEC375((bool)((((int32_t)L_0) == ((int32_t)0))? 1 : 0), _stringLiteral822EDE75F5970A676E660AB56E0A62A92F681766, NULL);
		return;
	}
}
// System.Void Grpc.Core.Metadata::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Metadata__cctor_mCE8B71AA4A6DA71845CB656BDFD10E69605219E2 (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0 = (Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06*)il2cpp_codegen_object_new(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		Metadata__ctor_mAA8F9E779B141794B1A58388BE41D32F97B0085A(L_0, NULL);
		NullCheck(L_0);
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_1;
		L_1 = Metadata_Freeze_m6A755BCE0A04B4A1B690D05277D90BBD1F3A4AD1(L_0, NULL);
		((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___Empty_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___Empty_1), (void*)L_1);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_2;
		L_2 = Encoding_get_ASCII_mCC61B512D320FD4E2E71CC0DFDF8DDF3CD215C65(NULL);
		((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___EncodingASCII_3 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___EncodingASCII_3), (void*)L_2);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Metadata/Entry::.ctor(System.String,System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Entry__ctor_m02C3B36E523E4E0215CB3E0A1EC30A906A6EF27E (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, String_t* ___1_value, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___2_valueBytes, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_key;
		__this->___key_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___key_0), (void*)L_0);
		String_t* L_1 = ___1_value;
		__this->___value_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___value_1), (void*)L_1);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_2 = ___2_valueBytes;
		__this->___valueBytes_2 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___valueBytes_2), (void*)L_2);
		return;
	}
}
// System.Void Grpc.Core.Metadata/Entry::.ctor(System.String,System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Entry__ctor_m7F675669248FB0A305E5B705BEC9550446AF5B0A (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___1_valueBytes, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_m2BEB1EC4BC44E79026DB6011E1E94018C77954EE_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral8E8AE59D86EF9B6862311A1970B81D68866956AB);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralE33CCB09E2FF8EF27D0D937E4C19A6936680A454);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_key;
		il2cpp_codegen_runtime_class_init_inline(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		String_t* L_1;
		L_1 = Entry_NormalizeKey_m87C9D5E6C68C229764D85F62077CA4EA6BA3EE3C(L_0, NULL);
		__this->___key_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___key_0), (void*)L_1);
		String_t* L_2 = __this->___key_0;
		bool L_3;
		L_3 = Entry_HasBinaryHeaderSuffix_m7BCA7E3AAB2BE2C598460A2E8A3EEEEFDFD5DBA7(L_2, NULL);
		GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497(L_3, _stringLiteral8E8AE59D86EF9B6862311A1970B81D68866956AB, NULL);
		__this->___value_1 = (String_t*)NULL;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___value_1), (void*)(String_t*)NULL);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_4 = ___1_valueBytes;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_5;
		L_5 = GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_m2BEB1EC4BC44E79026DB6011E1E94018C77954EE(L_4, _stringLiteralE33CCB09E2FF8EF27D0D937E4C19A6936680A454, GrpcPreconditions_CheckNotNull_TisByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_m2BEB1EC4BC44E79026DB6011E1E94018C77954EE_RuntimeMethod_var);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_6 = ___1_valueBytes;
		NullCheck(L_6);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_7 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)SZArrayNew(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var, (uint32_t)((int32_t)(((RuntimeArray*)L_6)->max_length)));
		__this->___valueBytes_2 = L_7;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___valueBytes_2), (void*)L_7);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_8 = ___1_valueBytes;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_9 = __this->___valueBytes_2;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_10 = ___1_valueBytes;
		NullCheck(L_10);
		Buffer_BlockCopy_m2F7BC0C5BA97C500E3F87D5008718F797E02B358((RuntimeArray*)L_8, 0, (RuntimeArray*)L_9, 0, ((int32_t)(((RuntimeArray*)L_10)->max_length)), NULL);
		return;
	}
}
// System.Void Grpc.Core.Metadata/Entry::.ctor(System.String,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Entry__ctor_m06E649C4EA82D12900A074292784F649C3D0538D (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, String_t* ___1_value, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral46F273EF641E07D271D91E0DC24A4392582671F8);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral92ACAEB1D158C2D3AC9BB0C86CA905A5314FBBDC);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_key;
		il2cpp_codegen_runtime_class_init_inline(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		String_t* L_1;
		L_1 = Entry_NormalizeKey_m87C9D5E6C68C229764D85F62077CA4EA6BA3EE3C(L_0, NULL);
		__this->___key_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___key_0), (void*)L_1);
		String_t* L_2 = __this->___key_0;
		bool L_3;
		L_3 = Entry_HasBinaryHeaderSuffix_m7BCA7E3AAB2BE2C598460A2E8A3EEEEFDFD5DBA7(L_2, NULL);
		GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497((bool)((((int32_t)L_3) == ((int32_t)0))? 1 : 0), _stringLiteral92ACAEB1D158C2D3AC9BB0C86CA905A5314FBBDC, NULL);
		String_t* L_4 = ___1_value;
		String_t* L_5;
		L_5 = GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B(L_4, _stringLiteral46F273EF641E07D271D91E0DC24A4392582671F8, GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		__this->___value_1 = L_5;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___value_1), (void*)L_5);
		__this->___valueBytes_2 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)NULL;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___valueBytes_2), (void*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)NULL);
		return;
	}
}
// System.String Grpc.Core.Metadata/Entry::get_Key()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Entry_get_Key_mEB6512663A174A3A1E9F39A6FEEE711030DEA2B6 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___key_0;
		return L_0;
	}
}
// System.Byte[] Grpc.Core.Metadata/Entry::get_ValueBytes()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* Entry_get_ValueBytes_m8952343C41FE19A9A5190F2C15D71BDB3CF8F5B9 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* V_0 = NULL;
	{
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = __this->___valueBytes_2;
		if (L_0)
		{
			goto IL_0019;
		}
	}
	{
		il2cpp_codegen_runtime_class_init_inline(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_1 = ((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___EncodingASCII_3;
		String_t* L_2 = __this->___value_1;
		NullCheck(L_1);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_3;
		L_3 = VirtualFuncInvoker1< ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*, String_t* >::Invoke(17 /* System.Byte[] System.Text.Encoding::GetBytes(System.String) */, L_1, L_2);
		return L_3;
	}

IL_0019:
	{
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_4 = __this->___valueBytes_2;
		NullCheck(L_4);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_5 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)SZArrayNew(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var, (uint32_t)((int32_t)(((RuntimeArray*)L_4)->max_length)));
		V_0 = L_5;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_6 = __this->___valueBytes_2;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_7 = V_0;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_8 = __this->___valueBytes_2;
		NullCheck(L_8);
		Buffer_BlockCopy_m2F7BC0C5BA97C500E3F87D5008718F797E02B358((RuntimeArray*)L_6, 0, (RuntimeArray*)L_7, 0, ((int32_t)(((RuntimeArray*)L_8)->max_length)), NULL);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_9 = V_0;
		return L_9;
	}
}
// System.String Grpc.Core.Metadata/Entry::get_Value()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Entry_get_Value_m1C0D0600AFC16B36093ED5B021335B952C4FD696 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral330132604A8C7C73B4775BFE93C8FBEC4857F022);
		s_Il2CppMethodInitialized = true;
	}
	{
		bool L_0;
		L_0 = Entry_get_IsBinary_m0A897895938055DB3CCBECB3B3AD72DC596D6E19(__this, NULL);
		GrpcPreconditions_CheckState_mCDE04614CE69128AF99EE030B5F83D386FDEC375((bool)((((int32_t)L_0) == ((int32_t)0))? 1 : 0), _stringLiteral330132604A8C7C73B4775BFE93C8FBEC4857F022, NULL);
		String_t* L_1 = __this->___value_1;
		return L_1;
	}
}
// System.Boolean Grpc.Core.Metadata/Entry::get_IsBinary()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_get_IsBinary_m0A897895938055DB3CCBECB3B3AD72DC596D6E19 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___value_1;
		return (bool)((((RuntimeObject*)(String_t*)L_0) == ((RuntimeObject*)(RuntimeObject*)NULL))? 1 : 0);
	}
}
// System.String Grpc.Core.Metadata/Entry::ToString()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Entry_ToString_m4551AEEDBD41CF6D5608DD8A3073EEAC65E64766 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral61B4338BE86CB9213EE71FAD62B393B8922755E8);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralA00F3DF646A4521C1DC3F40012285F4CA6DB6A43);
		s_Il2CppMethodInitialized = true;
	}
	{
		bool L_0;
		L_0 = Entry_get_IsBinary_m0A897895938055DB3CCBECB3B3AD72DC596D6E19(__this, NULL);
		if (!L_0)
		{
			goto IL_001f;
		}
	}
	{
		String_t* L_1 = __this->___key_0;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_2 = __this->___valueBytes_2;
		String_t* L_3;
		L_3 = String_Format_mFB7DA489BD99F4670881FF50EC017BFB0A5C0987(_stringLiteralA00F3DF646A4521C1DC3F40012285F4CA6DB6A43, L_1, (RuntimeObject*)L_2, NULL);
		return L_3;
	}

IL_001f:
	{
		String_t* L_4 = __this->___key_0;
		String_t* L_5 = __this->___value_1;
		String_t* L_6;
		L_6 = String_Format_mFB7DA489BD99F4670881FF50EC017BFB0A5C0987(_stringLiteral61B4338BE86CB9213EE71FAD62B393B8922755E8, L_4, L_5, NULL);
		return L_6;
	}
}
// System.Byte[] Grpc.Core.Metadata/Entry::GetSerializedValueUnsafe()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* Entry_GetSerializedValueUnsafe_m1A6C8DFE16462D534EC8906848F1281C95349B24 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* G_B2_0 = NULL;
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* G_B1_0 = NULL;
	{
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = __this->___valueBytes_2;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_1 = L_0;
		G_B1_0 = L_1;
		if (L_1)
		{
			G_B2_0 = L_1;
			goto IL_001a;
		}
	}
	{
		il2cpp_codegen_runtime_class_init_inline(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_2 = ((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___EncodingASCII_3;
		String_t* L_3 = __this->___value_1;
		NullCheck(L_2);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_4;
		L_4 = VirtualFuncInvoker1< ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*, String_t* >::Invoke(17 /* System.Byte[] System.Text.Encoding::GetBytes(System.String) */, L_2, L_3);
		G_B2_0 = L_4;
	}

IL_001a:
	{
		return G_B2_0;
	}
}
// System.Boolean Grpc.Core.Metadata/Entry::KeyEqualsIgnoreCase(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_KeyEqualsIgnoreCase_m5765029234BADCFDF40F5E1BD2E4CF3313D10A27 (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* __this, String_t* ___0_key, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___key_0;
		String_t* L_1 = ___0_key;
		bool L_2;
		L_2 = String_Equals_mCC34895D0DB2AD440C9D8767032215BC86B5C48B(L_0, L_1, 5, NULL);
		return L_2;
	}
}
// Grpc.Core.Metadata/Entry Grpc.Core.Metadata/Entry::CreateUnsafe(System.String,System.IntPtr,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* Entry_CreateUnsafe_m38E24B330600F7B46F675847EFBFEE9108EA54FC (String_t* ___0_key, intptr_t ___1_source, int32_t ___2_length, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Marshal_tD976A56A90263C3CE2B780D4B1CADADE2E70B4A7_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* V_0 = NULL;
	String_t* V_1 = NULL;
	{
		String_t* L_0 = ___0_key;
		il2cpp_codegen_runtime_class_init_inline(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		bool L_1;
		L_1 = Entry_HasBinaryHeaderSuffix_m7BCA7E3AAB2BE2C598460A2E8A3EEEEFDFD5DBA7(L_0, NULL);
		if (!L_1)
		{
			goto IL_002c;
		}
	}
	{
		int32_t L_2 = ___2_length;
		if (L_2)
		{
			goto IL_0013;
		}
	}
	{
		il2cpp_codegen_runtime_class_init_inline(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_3 = ((Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_StaticFields*)il2cpp_codegen_static_fields_for(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var))->___EmptyByteArray_3;
		V_0 = L_3;
		goto IL_0023;
	}

IL_0013:
	{
		int32_t L_4 = ___2_length;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_5 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)SZArrayNew(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var, (uint32_t)L_4);
		V_0 = L_5;
		intptr_t L_6 = ___1_source;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_7 = V_0;
		int32_t L_8 = ___2_length;
		il2cpp_codegen_runtime_class_init_inline(Marshal_tD976A56A90263C3CE2B780D4B1CADADE2E70B4A7_il2cpp_TypeInfo_var);
		Marshal_Copy_mF7402FFDB520EA1B8D1C32B368DBEE4B13F1BE77(L_6, L_7, 0, L_8, NULL);
	}

IL_0023:
	{
		String_t* L_9 = ___0_key;
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_10 = V_0;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_11 = (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*)il2cpp_codegen_object_new(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		NullCheck(L_11);
		Entry__ctor_m02C3B36E523E4E0215CB3E0A1EC30A906A6EF27E(L_11, L_9, (String_t*)NULL, L_10, NULL);
		return L_11;
	}

IL_002c:
	{
		il2cpp_codegen_runtime_class_init_inline(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_12 = ((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___EncodingASCII_3;
		intptr_t L_13 = ___1_source;
		int32_t L_14 = ___2_length;
		String_t* L_15;
		L_15 = EncodingExtensions_GetString_m5290CA29398B11EA0507EEEBE965E7E78137EEFF_inline(L_12, L_13, L_14, NULL);
		V_1 = L_15;
		String_t* L_16 = ___0_key;
		String_t* L_17 = V_1;
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_18 = (Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB*)il2cpp_codegen_object_new(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		NullCheck(L_18);
		Entry__ctor_m02C3B36E523E4E0215CB3E0A1EC30A906A6EF27E(L_18, L_16, L_17, (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)NULL, NULL);
		return L_18;
	}
}
// System.String Grpc.Core.Metadata/Entry::NormalizeKey(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Entry_NormalizeKey_m87C9D5E6C68C229764D85F62077CA4EA6BA3EE3C (String_t* ___0_key, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral0054B3392632CC246C5CE404303EC971B31E150D);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralE7D028CCE3B6E7B61AE2C752D7AE970DA04AB7C6);
		s_Il2CppMethodInitialized = true;
	}
	bool V_0 = false;
	{
		String_t* L_0 = ___0_key;
		String_t* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B(L_0, _stringLiteralE7D028CCE3B6E7B61AE2C752D7AE970DA04AB7C6, GrpcPreconditions_CheckNotNull_TisString_t_mF6AB788E4C0D5889F72E245022E205D2AAAC368B_RuntimeMethod_var);
		String_t* L_2 = ___0_key;
		il2cpp_codegen_runtime_class_init_inline(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		bool L_3;
		L_3 = Entry_IsValidKey_m564FB442C0B8E35F085DAA3C3C44C400B2FE4EE9(L_2, (&V_0), NULL);
		GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497(L_3, _stringLiteral0054B3392632CC246C5CE404303EC971B31E150D, NULL);
		bool L_4 = V_0;
		if (!L_4)
		{
			goto IL_0023;
		}
	}
	{
		String_t* L_5 = ___0_key;
		return L_5;
	}

IL_0023:
	{
		String_t* L_6 = ___0_key;
		NullCheck(L_6);
		String_t* L_7;
		L_7 = String_ToLowerInvariant_mBE32C93DE27C5353FEA3FA654FC1DDBE3D0EB0F2(L_6, NULL);
		return L_7;
	}
}
// System.Boolean Grpc.Core.Metadata/Entry::IsValidKey(System.String,System.Boolean&)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_IsValidKey_m564FB442C0B8E35F085DAA3C3C44C400B2FE4EE9 (String_t* ___0_input, bool* ___1_isLowercase, const RuntimeMethod* method) 
{
	int32_t V_0 = 0;
	Il2CppChar V_1 = 0x0;
	{
		bool* L_0 = ___1_isLowercase;
		*((int8_t*)L_0) = (int8_t)1;
		V_0 = 0;
		goto IL_0047;
	}

IL_0007:
	{
		String_t* L_1 = ___0_input;
		int32_t L_2 = V_0;
		NullCheck(L_1);
		Il2CppChar L_3;
		L_3 = String_get_Chars_mC49DF0CD2D3BE7BE97B3AD9C995BE3094F8E36D3(L_1, L_2, NULL);
		V_1 = L_3;
		Il2CppChar L_4 = V_1;
		if ((((int32_t)((int32_t)97)) > ((int32_t)L_4)))
		{
			goto IL_0019;
		}
	}
	{
		Il2CppChar L_5 = V_1;
		if ((((int32_t)L_5) <= ((int32_t)((int32_t)122))))
		{
			goto IL_0043;
		}
	}

IL_0019:
	{
		Il2CppChar L_6 = V_1;
		if ((((int32_t)((int32_t)48)) > ((int32_t)L_6)))
		{
			goto IL_0023;
		}
	}
	{
		Il2CppChar L_7 = V_1;
		if ((((int32_t)L_7) <= ((int32_t)((int32_t)57))))
		{
			goto IL_0043;
		}
	}

IL_0023:
	{
		Il2CppChar L_8 = V_1;
		if ((((int32_t)L_8) == ((int32_t)((int32_t)46))))
		{
			goto IL_0043;
		}
	}
	{
		Il2CppChar L_9 = V_1;
		if ((((int32_t)L_9) == ((int32_t)((int32_t)95))))
		{
			goto IL_0043;
		}
	}
	{
		Il2CppChar L_10 = V_1;
		if ((((int32_t)L_10) == ((int32_t)((int32_t)45))))
		{
			goto IL_0043;
		}
	}
	{
		Il2CppChar L_11 = V_1;
		if ((((int32_t)((int32_t)65)) > ((int32_t)L_11)))
		{
			goto IL_0041;
		}
	}
	{
		Il2CppChar L_12 = V_1;
		if ((((int32_t)L_12) > ((int32_t)((int32_t)90))))
		{
			goto IL_0041;
		}
	}
	{
		bool* L_13 = ___1_isLowercase;
		*((int8_t*)L_13) = (int8_t)0;
		goto IL_0043;
	}

IL_0041:
	{
		return (bool)0;
	}

IL_0043:
	{
		int32_t L_14 = V_0;
		V_0 = ((int32_t)il2cpp_codegen_add(L_14, 1));
	}

IL_0047:
	{
		int32_t L_15 = V_0;
		String_t* L_16 = ___0_input;
		NullCheck(L_16);
		int32_t L_17;
		L_17 = String_get_Length_m42625D67623FA5CC7A44D47425CE86FB946542D2_inline(L_16, NULL);
		if ((((int32_t)L_15) < ((int32_t)L_17)))
		{
			goto IL_0007;
		}
	}
	{
		return (bool)1;
	}
}
// System.Boolean Grpc.Core.Metadata/Entry::HasBinaryHeaderSuffix(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool Entry_HasBinaryHeaderSuffix_m7BCA7E3AAB2BE2C598460A2E8A3EEEEFDFD5DBA7 (String_t* ___0_key, const RuntimeMethod* method) 
{
	int32_t V_0 = 0;
	{
		String_t* L_0 = ___0_key;
		NullCheck(L_0);
		int32_t L_1;
		L_1 = String_get_Length_m42625D67623FA5CC7A44D47425CE86FB946542D2_inline(L_0, NULL);
		V_0 = L_1;
		int32_t L_2 = V_0;
		if ((((int32_t)L_2) < ((int32_t)4)))
		{
			goto IL_0041;
		}
	}
	{
		String_t* L_3 = ___0_key;
		int32_t L_4 = V_0;
		NullCheck(L_3);
		Il2CppChar L_5;
		L_5 = String_get_Chars_mC49DF0CD2D3BE7BE97B3AD9C995BE3094F8E36D3(L_3, ((int32_t)il2cpp_codegen_subtract(L_4, 4)), NULL);
		if ((!(((uint32_t)L_5) == ((uint32_t)((int32_t)45)))))
		{
			goto IL_0041;
		}
	}
	{
		String_t* L_6 = ___0_key;
		int32_t L_7 = V_0;
		NullCheck(L_6);
		Il2CppChar L_8;
		L_8 = String_get_Chars_mC49DF0CD2D3BE7BE97B3AD9C995BE3094F8E36D3(L_6, ((int32_t)il2cpp_codegen_subtract(L_7, 3)), NULL);
		if ((!(((uint32_t)L_8) == ((uint32_t)((int32_t)98)))))
		{
			goto IL_0041;
		}
	}
	{
		String_t* L_9 = ___0_key;
		int32_t L_10 = V_0;
		NullCheck(L_9);
		Il2CppChar L_11;
		L_11 = String_get_Chars_mC49DF0CD2D3BE7BE97B3AD9C995BE3094F8E36D3(L_9, ((int32_t)il2cpp_codegen_subtract(L_10, 2)), NULL);
		if ((!(((uint32_t)L_11) == ((uint32_t)((int32_t)105)))))
		{
			goto IL_0041;
		}
	}
	{
		String_t* L_12 = ___0_key;
		int32_t L_13 = V_0;
		NullCheck(L_12);
		Il2CppChar L_14;
		L_14 = String_get_Chars_mC49DF0CD2D3BE7BE97B3AD9C995BE3094F8E36D3(L_12, ((int32_t)il2cpp_codegen_subtract(L_13, 1)), NULL);
		if ((!(((uint32_t)L_14) == ((uint32_t)((int32_t)110)))))
		{
			goto IL_0041;
		}
	}
	{
		return (bool)1;
	}

IL_0041:
	{
		return (bool)0;
	}
}
// System.Void Grpc.Core.Metadata/Entry::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Entry__cctor_mE5D543A410C4F3B958F055A721429B3C7A03BAD0 (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* L_0 = (ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031*)SZArrayNew(ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031_il2cpp_TypeInfo_var, (uint32_t)0);
		((Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_StaticFields*)il2cpp_codegen_static_fields_for(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var))->___EmptyByteArray_3 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&((Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_StaticFields*)il2cpp_codegen_static_fields_for(Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB_il2cpp_TypeInfo_var))->___EmptyByteArray_3), (void*)L_0);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Metadata/<GetAll>d__11::.ctor(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CGetAllU3Ed__11__ctor_m35967E7C5537A498542DC7F352BD828EA734D4A0 (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, int32_t ___0_U3CU3E1__state, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		int32_t L_0 = ___0_U3CU3E1__state;
		__this->___U3CU3E1__state_0 = L_0;
		int32_t L_1;
		L_1 = Environment_get_CurrentManagedThreadId_m66483AADCCC13272EBDCD94D31D2E52603C24BDF(NULL);
		__this->___U3CU3El__initialThreadId_2 = L_1;
		return;
	}
}
// System.Void Grpc.Core.Metadata/<GetAll>d__11::System.IDisposable.Dispose()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CGetAllU3Ed__11_System_IDisposable_Dispose_mF2FA1AC536A8F19AE6542A49D19AD97EEA6DA56B (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) 
{
	{
		return;
	}
}
// System.Boolean Grpc.Core.Metadata/<GetAll>d__11::MoveNext()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool U3CGetAllU3Ed__11_MoveNext_mF5C84DF9EBFC1CD6F9AF33D346665FDB53AF88A4 (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	int32_t V_0 = 0;
	Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* V_1 = NULL;
	int32_t V_2 = 0;
	{
		int32_t L_0 = __this->___U3CU3E1__state_0;
		V_0 = L_0;
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_1 = __this->___U3CU3E4__this_3;
		V_1 = L_1;
		int32_t L_2 = V_0;
		if (!L_2)
		{
			goto IL_0017;
		}
	}
	{
		int32_t L_3 = V_0;
		if ((((int32_t)L_3) == ((int32_t)1)))
		{
			goto IL_0065;
		}
	}
	{
		return (bool)0;
	}

IL_0017:
	{
		__this->___U3CU3E1__state_0 = (-1);
		__this->___U3CiU3E5__2_6 = 0;
		goto IL_007c;
	}

IL_0027:
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_4 = V_1;
		NullCheck(L_4);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_5 = L_4->___entries_4;
		int32_t L_6 = __this->___U3CiU3E5__2_6;
		NullCheck(L_5);
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_7;
		L_7 = List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9(L_5, L_6, List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		String_t* L_8 = __this->___key_4;
		NullCheck(L_7);
		bool L_9;
		L_9 = Entry_KeyEqualsIgnoreCase_m5765029234BADCFDF40F5E1BD2E4CF3313D10A27(L_7, L_8, NULL);
		if (!L_9)
		{
			goto IL_006c;
		}
	}
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_10 = V_1;
		NullCheck(L_10);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_11 = L_10->___entries_4;
		int32_t L_12 = __this->___U3CiU3E5__2_6;
		NullCheck(L_11);
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_13;
		L_13 = List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9(L_11, L_12, List_1_get_Item_m12120F6C376ADCC243FBC8D02C4602BA749C4DC9_RuntimeMethod_var);
		__this->___U3CU3E2__current_1 = L_13;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___U3CU3E2__current_1), (void*)L_13);
		__this->___U3CU3E1__state_0 = 1;
		return (bool)1;
	}

IL_0065:
	{
		__this->___U3CU3E1__state_0 = (-1);
	}

IL_006c:
	{
		int32_t L_14 = __this->___U3CiU3E5__2_6;
		V_2 = L_14;
		int32_t L_15 = V_2;
		__this->___U3CiU3E5__2_6 = ((int32_t)il2cpp_codegen_add(L_15, 1));
	}

IL_007c:
	{
		int32_t L_16 = __this->___U3CiU3E5__2_6;
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_17 = V_1;
		NullCheck(L_17);
		List_1_t9FE6338CFB4FD5AFFFABF90B6DD6087DBE808337* L_18 = L_17->___entries_4;
		NullCheck(L_18);
		int32_t L_19;
		L_19 = List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_inline(L_18, List_1_get_Count_mB58A562C1FE074B9C495BAB15069ECE83B3BD3B5_RuntimeMethod_var);
		if ((((int32_t)L_16) < ((int32_t)L_19)))
		{
			goto IL_0027;
		}
	}
	{
		return (bool)0;
	}
}
// Grpc.Core.Metadata/Entry Grpc.Core.Metadata/<GetAll>d__11::System.Collections.Generic.IEnumerator<Grpc.Core.Metadata.Entry>.get_Current()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* U3CGetAllU3Ed__11_System_Collections_Generic_IEnumeratorU3CGrpc_Core_Metadata_EntryU3E_get_Current_m017C788EDD11E1B22F70C2D0DA3CE3094DBC6A78 (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) 
{
	{
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_0 = __this->___U3CU3E2__current_1;
		return L_0;
	}
}
// System.Void Grpc.Core.Metadata/<GetAll>d__11::System.Collections.IEnumerator.Reset()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void U3CGetAllU3Ed__11_System_Collections_IEnumerator_Reset_mC30E2AFCFE8F4319C587AE4B69E7C8DAE429D37D (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) 
{
	{
		NotSupportedException_t1429765983D409BD2986508963C98D214E4EBF4A* L_0 = (NotSupportedException_t1429765983D409BD2986508963C98D214E4EBF4A*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&NotSupportedException_t1429765983D409BD2986508963C98D214E4EBF4A_il2cpp_TypeInfo_var)));
		NullCheck(L_0);
		NotSupportedException__ctor_m1398D0CDE19B36AA3DE9392879738C1EA2439CDF(L_0, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_0, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&U3CGetAllU3Ed__11_System_Collections_IEnumerator_Reset_mC30E2AFCFE8F4319C587AE4B69E7C8DAE429D37D_RuntimeMethod_var)));
	}
}
// System.Object Grpc.Core.Metadata/<GetAll>d__11::System.Collections.IEnumerator.get_Current()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* U3CGetAllU3Ed__11_System_Collections_IEnumerator_get_Current_m31B746B3FA5985A3A59B4A95A124B57A43CAE7BA (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) 
{
	{
		Entry_tD4A927251AFDB94306DBF05F34FE7346944555BB* L_0 = __this->___U3CU3E2__current_1;
		return L_0;
	}
}
// System.Collections.Generic.IEnumerator`1<Grpc.Core.Metadata/Entry> Grpc.Core.Metadata/<GetAll>d__11::System.Collections.Generic.IEnumerable<Grpc.Core.Metadata.Entry>.GetEnumerator()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* U3CGetAllU3Ed__11_System_Collections_Generic_IEnumerableU3CGrpc_Core_Metadata_EntryU3E_GetEnumerator_m2928F9F872395CA6B69AFCA12C4C0477DBACF83B (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* V_0 = NULL;
	{
		int32_t L_0 = __this->___U3CU3E1__state_0;
		if ((!(((uint32_t)L_0) == ((uint32_t)((int32_t)-2)))))
		{
			goto IL_0022;
		}
	}
	{
		int32_t L_1 = __this->___U3CU3El__initialThreadId_2;
		int32_t L_2;
		L_2 = Environment_get_CurrentManagedThreadId_m66483AADCCC13272EBDCD94D31D2E52603C24BDF(NULL);
		if ((!(((uint32_t)L_1) == ((uint32_t)L_2))))
		{
			goto IL_0022;
		}
	}
	{
		__this->___U3CU3E1__state_0 = 0;
		V_0 = __this;
		goto IL_0035;
	}

IL_0022:
	{
		U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* L_3 = (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3*)il2cpp_codegen_object_new(U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3_il2cpp_TypeInfo_var);
		NullCheck(L_3);
		U3CGetAllU3Ed__11__ctor_m35967E7C5537A498542DC7F352BD828EA734D4A0(L_3, 0, NULL);
		V_0 = L_3;
		U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* L_4 = V_0;
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_5 = __this->___U3CU3E4__this_3;
		NullCheck(L_4);
		L_4->___U3CU3E4__this_3 = L_5;
		Il2CppCodeGenWriteBarrier((void**)(&L_4->___U3CU3E4__this_3), (void*)L_5);
	}

IL_0035:
	{
		U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* L_6 = V_0;
		String_t* L_7 = __this->___U3CU3E3__key_5;
		NullCheck(L_6);
		L_6->___key_4 = L_7;
		Il2CppCodeGenWriteBarrier((void**)(&L_6->___key_4), (void*)L_7);
		U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* L_8 = V_0;
		return L_8;
	}
}
// System.Collections.IEnumerator Grpc.Core.Metadata/<GetAll>d__11::System.Collections.IEnumerable.GetEnumerator()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* U3CGetAllU3Ed__11_System_Collections_IEnumerable_GetEnumerator_m8805A162B24DD2450BF630462931A7084F9B8D42 (U3CGetAllU3Ed__11_t2E4FF8E98A9E68EBC735BDBE9E0B0131C282DBF3* __this, const RuntimeMethod* method) 
{
	{
		RuntimeObject* L_0;
		L_0 = U3CGetAllU3Ed__11_System_Collections_Generic_IEnumerableU3CGrpc_Core_Metadata_EntryU3E_GetEnumerator_m2928F9F872395CA6B69AFCA12C4C0477DBACF83B(__this, NULL);
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.RpcException::.ctor(Grpc.Core.Status)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void RpcException__ctor_m06D2314D26D2BC163E2753D66137EC3DFEBE3E56 (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___0_status, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_0 = ___0_status;
		il2cpp_codegen_runtime_class_init_inline(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_1 = ((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___Empty_1;
		String_t* L_2;
		L_2 = Status_ToString_m85A7C885F4E22BC641454E680EB9AB02A8A79036((&___0_status), NULL);
		RpcException__ctor_m5602C523B475531625FDDC82B574BE2558E1DF4F(__this, L_0, L_1, L_2, NULL);
		return;
	}
}
// System.Void Grpc.Core.RpcException::.ctor(Grpc.Core.Status,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void RpcException__ctor_m7C84D8EA42397814A14C2BAF4F98B697D25BE63F (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___0_status, String_t* ___1_message, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_0 = ___0_status;
		il2cpp_codegen_runtime_class_init_inline(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var);
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_1 = ((Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_StaticFields*)il2cpp_codegen_static_fields_for(Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_il2cpp_TypeInfo_var))->___Empty_1;
		String_t* L_2 = ___1_message;
		RpcException__ctor_m5602C523B475531625FDDC82B574BE2558E1DF4F(__this, L_0, L_1, L_2, NULL);
		return;
	}
}
// System.Void Grpc.Core.RpcException::.ctor(Grpc.Core.Status,Grpc.Core.Metadata)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void RpcException__ctor_m54B23AE0A9BC985AA9D8400D0E0B4D2D72611DCA (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___0_status, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_trailers, const RuntimeMethod* method) 
{
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_0 = ___0_status;
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_1 = ___1_trailers;
		String_t* L_2;
		L_2 = Status_ToString_m85A7C885F4E22BC641454E680EB9AB02A8A79036((&___0_status), NULL);
		RpcException__ctor_m5602C523B475531625FDDC82B574BE2558E1DF4F(__this, L_0, L_1, L_2, NULL);
		return;
	}
}
// System.Void Grpc.Core.RpcException::.ctor(Grpc.Core.Status,Grpc.Core.Metadata,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void RpcException__ctor_m5602C523B475531625FDDC82B574BE2558E1DF4F (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___0_status, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___1_trailers, String_t* ___2_message, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Exception_t_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisMetadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_m3DDAE807CB04B4ADAC6FB166B7EC34BE2A3720AE_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		String_t* L_0 = ___2_message;
		il2cpp_codegen_runtime_class_init_inline(Exception_t_il2cpp_TypeInfo_var);
		Exception__ctor_m9B2BD92CD68916245A75109105D9071C9D430E7F(__this, L_0, NULL);
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_1 = ___0_status;
		__this->___status_18 = L_1;
		Il2CppCodeGenWriteBarrier((void**)&(((&__this->___status_18))->___U3CDetailU3Ek__BackingField_3), (void*)NULL);
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&(((&__this->___status_18))->___U3CDebugExceptionU3Ek__BackingField_4), (void*)NULL);
		#endif
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_2 = ___1_trailers;
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_3;
		L_3 = GrpcPreconditions_CheckNotNull_TisMetadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_m3DDAE807CB04B4ADAC6FB166B7EC34BE2A3720AE(L_2, GrpcPreconditions_CheckNotNull_TisMetadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06_m3DDAE807CB04B4ADAC6FB166B7EC34BE2A3720AE_RuntimeMethod_var);
		__this->___trailers_19 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___trailers_19), (void*)L_3);
		return;
	}
}
// Grpc.Core.Status Grpc.Core.RpcException::get_Status()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 RpcException_get_Status_m97BB066C8206825C559D30E006CBA1D665537550 (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, const RuntimeMethod* method) 
{
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_0 = __this->___status_18;
		return L_0;
	}
}
// Grpc.Core.StatusCode Grpc.Core.RpcException::get_StatusCode()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t RpcException_get_StatusCode_mA13201AD06B758B5056B3983EA4C4D107476D2AE (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, const RuntimeMethod* method) 
{
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* L_0 = (&__this->___status_18);
		int32_t L_1;
		L_1 = Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386_inline(L_0, NULL);
		return L_1;
	}
}
// Grpc.Core.Metadata Grpc.Core.RpcException::get_Trailers()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* RpcException_get_Trailers_m7247398D8F41BAD54982A56E18D746FA62622C9F (RpcException_tA6335FD6A3ABCA08A5A51D7C3740BFAA176C5A72* __this, const RuntimeMethod* method) 
{
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0 = __this->___trailers_19;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.SerializationContext::Complete(System.Byte[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SerializationContext_Complete_m5A51FF9AD59C9E706ACCC9FDD7BAF5E747709B9E (SerializationContext_tB58A48F5BD333BD147B799711A88A26662D09ADB* __this, ByteU5BU5D_tA6237BF417AE52AD70CFB4EF24A7A82613DF9031* ___0_payload, const RuntimeMethod* method) 
{
	{
		NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8* L_0 = (NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8_il2cpp_TypeInfo_var)));
		NullCheck(L_0);
		NotImplementedException__ctor_mDAB47BC6BD0E342E8F2171E5CABE3E67EA049F1C(L_0, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_0, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&SerializationContext_Complete_m5A51FF9AD59C9E706ACCC9FDD7BAF5E747709B9E_RuntimeMethod_var)));
	}
}
// System.Buffers.IBufferWriter`1<System.Byte> Grpc.Core.SerializationContext::GetBufferWriter()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* SerializationContext_GetBufferWriter_m5CAC81A9C524E847470E05C14D5359E7804EF265 (SerializationContext_tB58A48F5BD333BD147B799711A88A26662D09ADB* __this, const RuntimeMethod* method) 
{
	{
		NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8* L_0 = (NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8_il2cpp_TypeInfo_var)));
		NullCheck(L_0);
		NotImplementedException__ctor_mDAB47BC6BD0E342E8F2171E5CABE3E67EA049F1C(L_0, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_0, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&SerializationContext_GetBufferWriter_m5CAC81A9C524E847470E05C14D5359E7804EF265_RuntimeMethod_var)));
	}
}
// System.Void Grpc.Core.SerializationContext::SetPayloadLength(System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SerializationContext_SetPayloadLength_m8A87C8D5017C02B51744A8F8FD3D201873646B8A (SerializationContext_tB58A48F5BD333BD147B799711A88A26662D09ADB* __this, int32_t ___0_payloadLength, const RuntimeMethod* method) 
{
	{
		return;
	}
}
// System.Void Grpc.Core.SerializationContext::Complete()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SerializationContext_Complete_m3C0A349F0B19FE856B4EFB5C7E7569D74025C330 (SerializationContext_tB58A48F5BD333BD147B799711A88A26662D09ADB* __this, const RuntimeMethod* method) 
{
	{
		NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8* L_0 = (NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&NotImplementedException_t6366FE4DCF15094C51F4833B91A2AE68D4DA90E8_il2cpp_TypeInfo_var)));
		NullCheck(L_0);
		NotImplementedException__ctor_mDAB47BC6BD0E342E8F2171E5CABE3E67EA049F1C(L_0, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_0, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&SerializationContext_Complete_m3C0A349F0B19FE856B4EFB5C7E7569D74025C330_RuntimeMethod_var)));
	}
}
// System.Void Grpc.Core.SerializationContext::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SerializationContext__ctor_m602A3AB63C53589CC88F44BC281AC3C809B23919 (SerializationContext_tB58A48F5BD333BD147B799711A88A26662D09ADB* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ServerCallContext::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ServerCallContext__ctor_m16B7E49116A536AAB0BA703D19E477F4586D47A3 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
// System.Threading.Tasks.Task Grpc.Core.ServerCallContext::WriteResponseHeadersAsync(Grpc.Core.Metadata)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* ServerCallContext_WriteResponseHeadersAsync_m14FF649718F38620D7933A2E72D9C6B582D7BFC6 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ___0_responseHeaders, const RuntimeMethod* method) 
{
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0 = ___0_responseHeaders;
		Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572* L_1;
		L_1 = VirtualFuncInvoker1< Task_t751C4CC3ECD055BABA8A0B6A5DFBB4283DCA8572*, Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(4 /* System.Threading.Tasks.Task Grpc.Core.ServerCallContext::WriteResponseHeadersAsyncCore(Grpc.Core.Metadata) */, __this, L_0);
		return L_1;
	}
}
// Grpc.Core.ContextPropagationToken Grpc.Core.ServerCallContext::CreatePropagationToken(Grpc.Core.ContextPropagationOptions)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* ServerCallContext_CreatePropagationToken_mD7F3E49F52BF57F7BE18476A45BF18C14D2383FC (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* ___0_options, const RuntimeMethod* method) 
{
	{
		ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* L_0 = ___0_options;
		ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* L_1;
		L_1 = VirtualFuncInvoker1< ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751*, ContextPropagationOptions_tDEE3192782649F768A5A3D1975F3E2CDF4FC5C6B* >::Invoke(5 /* Grpc.Core.ContextPropagationToken Grpc.Core.ServerCallContext::CreatePropagationTokenCore(Grpc.Core.ContextPropagationOptions) */, __this, L_0);
		return L_1;
	}
}
// System.String Grpc.Core.ServerCallContext::get_Method()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* ServerCallContext_get_Method_m87BB8C9AFA85168C1430D089565B1E28DA470A28 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0;
		L_0 = VirtualFuncInvoker0< String_t* >::Invoke(6 /* System.String Grpc.Core.ServerCallContext::get_MethodCore() */, __this);
		return L_0;
	}
}
// System.String Grpc.Core.ServerCallContext::get_Host()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* ServerCallContext_get_Host_mE2747BB735F95DCB0B3C42C93436670C9EC45997 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0;
		L_0 = VirtualFuncInvoker0< String_t* >::Invoke(7 /* System.String Grpc.Core.ServerCallContext::get_HostCore() */, __this);
		return L_0;
	}
}
// System.String Grpc.Core.ServerCallContext::get_Peer()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* ServerCallContext_get_Peer_m25DAF4EA8C421B64C2DA3AE90967D5F9F11963BE (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0;
		L_0 = VirtualFuncInvoker0< String_t* >::Invoke(8 /* System.String Grpc.Core.ServerCallContext::get_PeerCore() */, __this);
		return L_0;
	}
}
// System.DateTime Grpc.Core.ServerCallContext::get_Deadline()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D ServerCallContext_get_Deadline_m638F3CB24637BB26414B0DE34E4E13E1856D6DE3 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D L_0;
		L_0 = VirtualFuncInvoker0< DateTime_t66193957C73913903DDAD89FEDC46139BCA5802D >::Invoke(9 /* System.DateTime Grpc.Core.ServerCallContext::get_DeadlineCore() */, __this);
		return L_0;
	}
}
// Grpc.Core.Metadata Grpc.Core.ServerCallContext::get_RequestHeaders()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ServerCallContext_get_RequestHeaders_m40BBF7AB2BA8519A89AF7DE1AD0CB427EF8E6A26 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0;
		L_0 = VirtualFuncInvoker0< Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(10 /* Grpc.Core.Metadata Grpc.Core.ServerCallContext::get_RequestHeadersCore() */, __this);
		return L_0;
	}
}
// System.Threading.CancellationToken Grpc.Core.ServerCallContext::get_CancellationToken()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED ServerCallContext_get_CancellationToken_m0A9177B290C2F7B0A558C10A8C3B4D933D9851B1 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED L_0;
		L_0 = VirtualFuncInvoker0< CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED >::Invoke(11 /* System.Threading.CancellationToken Grpc.Core.ServerCallContext::get_CancellationTokenCore() */, __this);
		return L_0;
	}
}
// Grpc.Core.Metadata Grpc.Core.ServerCallContext::get_ResponseTrailers()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* ServerCallContext_get_ResponseTrailers_m1DC46A53532FBE95CAD8F0181F5A8C58E9DFC519 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0;
		L_0 = VirtualFuncInvoker0< Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* >::Invoke(12 /* Grpc.Core.Metadata Grpc.Core.ServerCallContext::get_ResponseTrailersCore() */, __this);
		return L_0;
	}
}
// Grpc.Core.Status Grpc.Core.ServerCallContext::get_Status()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ServerCallContext_get_Status_m5077C85D89DC754BDD0545358E2881501252DC62 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_0;
		L_0 = VirtualFuncInvoker0< Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 >::Invoke(13 /* Grpc.Core.Status Grpc.Core.ServerCallContext::get_StatusCore() */, __this);
		return L_0;
	}
}
// System.Void Grpc.Core.ServerCallContext::set_Status(Grpc.Core.Status)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ServerCallContext_set_Status_mB7C93AF533C2EAAA04862F09D4E381C3C6C8CD12 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 ___0_value, const RuntimeMethod* method) 
{
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_0 = ___0_value;
		VirtualActionInvoker1< Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 >::Invoke(14 /* System.Void Grpc.Core.ServerCallContext::set_StatusCore(Grpc.Core.Status) */, __this, L_0);
		return;
	}
}
// Grpc.Core.WriteOptions Grpc.Core.ServerCallContext::get_WriteOptions()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ServerCallContext_get_WriteOptions_m49926A2990BF56EDE8A97D212183EC42BAB66C2B (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* L_0;
		L_0 = VirtualFuncInvoker0< WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* >::Invoke(15 /* Grpc.Core.WriteOptions Grpc.Core.ServerCallContext::get_WriteOptionsCore() */, __this);
		return L_0;
	}
}
// System.Void Grpc.Core.ServerCallContext::set_WriteOptions(Grpc.Core.WriteOptions)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ServerCallContext_set_WriteOptions_m54573C980E87EEEC33CC79AAA4FA7292A8E01640 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* ___0_value, const RuntimeMethod* method) 
{
	{
		WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* L_0 = ___0_value;
		VirtualActionInvoker1< WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* >::Invoke(16 /* System.Void Grpc.Core.ServerCallContext::set_WriteOptionsCore(Grpc.Core.WriteOptions) */, __this, L_0);
		return;
	}
}
// Grpc.Core.AuthContext Grpc.Core.ServerCallContext::get_AuthContext()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* ServerCallContext_get_AuthContext_m04AFE4E31B9D5F935BBEC6AE41A7DD5B755AF994 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* L_0;
		L_0 = VirtualFuncInvoker0< AuthContext_t08949B8C5FF1CF77E7624E4F69BF3B1EE4C48B57* >::Invoke(17 /* Grpc.Core.AuthContext Grpc.Core.ServerCallContext::get_AuthContextCore() */, __this);
		return L_0;
	}
}
// System.Collections.Generic.IDictionary`2<System.Object,System.Object> Grpc.Core.ServerCallContext::get_UserState()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* ServerCallContext_get_UserState_m7716C9DE0BB505528D37BF8CD4BC21C9529B70E5 (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	{
		RuntimeObject* L_0;
		L_0 = VirtualFuncInvoker0< RuntimeObject* >::Invoke(18 /* System.Collections.Generic.IDictionary`2<System.Object,System.Object> Grpc.Core.ServerCallContext::get_UserStateCore() */, __this);
		return L_0;
	}
}
// System.Collections.Generic.IDictionary`2<System.Object,System.Object> Grpc.Core.ServerCallContext::get_UserStateCore()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* ServerCallContext_get_UserStateCore_m80A3A0D042D3D90C8A4B913C5515BA2D6A04E92B (ServerCallContext_t03C40610542F091E955B15B4AB225ED1A909C5E9* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* L_0 = __this->___userState_0;
		if (L_0)
		{
			goto IL_0013;
		}
	}
	{
		Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* L_1 = (Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA*)il2cpp_codegen_object_new(Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329(L_1, Dictionary_2__ctor_m5B32FBC624618211EB461D59CFBB10E987FD1329_RuntimeMethod_var);
		__this->___userState_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___userState_0), (void*)L_1);
	}

IL_0013:
	{
		Dictionary_2_t14FE4A752A83D53771C584E4C8D14E01F2AFD7BA* L_2 = __this->___userState_0;
		return L_2;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ServerServiceDefinition::.ctor(System.Collections.Generic.List`1<System.Action`1<Grpc.Core.ServiceBinderBase>>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ServerServiceDefinition__ctor_mF4BC7D215A7F6AECBA8865439697D6F8E2F9E405 (ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0* __this, List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* ___0_addMethodActions, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_AsReadOnly_mF5D40FE5DCA9ABF87F4D97EB90234195022B9440_RuntimeMethod_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* L_0 = ___0_addMethodActions;
		NullCheck(L_0);
		ReadOnlyCollection_1_tBC605066ABB00B74ED2389020F64275AA494E462* L_1;
		L_1 = List_1_AsReadOnly_mF5D40FE5DCA9ABF87F4D97EB90234195022B9440(L_0, List_1_AsReadOnly_mF5D40FE5DCA9ABF87F4D97EB90234195022B9440_RuntimeMethod_var);
		__this->___addMethodActions_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___addMethodActions_0), (void*)L_1);
		return;
	}
}
// System.Void Grpc.Core.ServerServiceDefinition::BindService(Grpc.Core.ServiceBinderBase)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ServerServiceDefinition_BindService_m5EA6F0C58F9FD0678F6833D71529B7FBE184BF31 (ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0* __this, ServiceBinderBase_tFB0E641375E302B3D9DEC054D9D9BA8197FC104A* ___0_serviceBinder, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IDisposable_t030E0496B4E0E4E4F086825007979AF51F7248C5_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IEnumerable_1_t7073586B577B76AB9907E9E8F21F45C2C364BC4F_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IEnumerator_1_tF7846B70B58A638138E3AFE67ED47A962B32B2E1_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IEnumerator_t7B609C2FFA6EB5167D9C62A0C32A21DE2F666DAA_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	RuntimeObject* V_0 = NULL;
	{
		RuntimeObject* L_0 = __this->___addMethodActions_0;
		NullCheck(L_0);
		RuntimeObject* L_1;
		L_1 = InterfaceFuncInvoker0< RuntimeObject* >::Invoke(0 /* System.Collections.Generic.IEnumerator`1<T> System.Collections.Generic.IEnumerable`1<System.Action`1<Grpc.Core.ServiceBinderBase>>::GetEnumerator() */, IEnumerable_1_t7073586B577B76AB9907E9E8F21F45C2C364BC4F_il2cpp_TypeInfo_var, L_0);
		V_0 = L_1;
	}
	{
		auto __finallyBlock = il2cpp::utils::Finally([&]
		{

FINALLY_0024:
			{// begin finally (depth: 1)
				{
					RuntimeObject* L_2 = V_0;
					if (!L_2)
					{
						goto IL_002d;
					}
				}
				{
					RuntimeObject* L_3 = V_0;
					NullCheck(L_3);
					InterfaceActionInvoker0::Invoke(0 /* System.Void System.IDisposable::Dispose() */, IDisposable_t030E0496B4E0E4E4F086825007979AF51F7248C5_il2cpp_TypeInfo_var, L_3);
				}

IL_002d:
				{
					return;
				}
			}// end finally (depth: 1)
		});
		try
		{// begin try (depth: 1)
			{
				goto IL_001a_1;
			}

IL_000e_1:
			{
				RuntimeObject* L_4 = V_0;
				NullCheck(L_4);
				Action_1_t805B4C224C6B55AF87988396EE027CA269CD12EE* L_5;
				L_5 = InterfaceFuncInvoker0< Action_1_t805B4C224C6B55AF87988396EE027CA269CD12EE* >::Invoke(0 /* T System.Collections.Generic.IEnumerator`1<System.Action`1<Grpc.Core.ServiceBinderBase>>::get_Current() */, IEnumerator_1_tF7846B70B58A638138E3AFE67ED47A962B32B2E1_il2cpp_TypeInfo_var, L_4);
				ServiceBinderBase_tFB0E641375E302B3D9DEC054D9D9BA8197FC104A* L_6 = ___0_serviceBinder;
				NullCheck(L_5);
				Action_1_Invoke_mD511836C3522F85EE4CD5D608A1F9D0FA9C3A674_inline(L_5, L_6, NULL);
			}

IL_001a_1:
			{
				RuntimeObject* L_7 = V_0;
				NullCheck(L_7);
				bool L_8;
				L_8 = InterfaceFuncInvoker0< bool >::Invoke(0 /* System.Boolean System.Collections.IEnumerator::MoveNext() */, IEnumerator_t7B609C2FFA6EB5167D9C62A0C32A21DE2F666DAA_il2cpp_TypeInfo_var, L_7);
				if (L_8)
				{
					goto IL_000e_1;
				}
			}
			{
				goto IL_002e;
			}
		}// end try (depth: 1)
		catch(Il2CppExceptionWrapper& e)
		{
			__finallyBlock.StoreException(e.ex);
		}
	}

IL_002e:
	{
		return;
	}
}
// Grpc.Core.ServerServiceDefinition/Builder Grpc.Core.ServerServiceDefinition::CreateBuilder()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32* ServerServiceDefinition_CreateBuilder_mA1E7CF4CE4D0D2AD1A3A6C9595502C50CDD555AA (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32* L_0 = (Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32*)il2cpp_codegen_object_new(Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		Builder__ctor_m93E6C2BE407A34F963268933CB7D6FEF59D9732E(L_0, NULL);
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ServerServiceDefinition/Builder::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Builder__ctor_m93E6C2BE407A34F963268933CB7D6FEF59D9732E (Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Dictionary_2__ctor_mC4F3DF292BAD88F4BF193C49CD689FAEBC4570A9_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1__ctor_m3339F822AD7AB85E44A3CE0FD82074C27CBB7903_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710* L_0 = (Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710*)il2cpp_codegen_object_new(Dictionary_2_tA348003A3C1CEFB3096E9D2A0BC7F1AC8EC4F710_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		Dictionary_2__ctor_mC4F3DF292BAD88F4BF193C49CD689FAEBC4570A9(L_0, Dictionary_2__ctor_mC4F3DF292BAD88F4BF193C49CD689FAEBC4570A9_RuntimeMethod_var);
		__this->___duplicateDetector_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___duplicateDetector_0), (void*)L_0);
		List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* L_1 = (List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5*)il2cpp_codegen_object_new(List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		List_1__ctor_m3339F822AD7AB85E44A3CE0FD82074C27CBB7903(L_1, List_1__ctor_m3339F822AD7AB85E44A3CE0FD82074C27CBB7903_RuntimeMethod_var);
		__this->___addMethodActions_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___addMethodActions_1), (void*)L_1);
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
// Grpc.Core.ServerServiceDefinition Grpc.Core.ServerServiceDefinition/Builder::Build()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0* Builder_Build_m09BE41A1247701FABFF203159FF8872EEC2A021F (Builder_tDC19B0BF69D67D0E1C6E9B7D9696F754AA822C32* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		List_1_t28B295E550D7F6C1924CEDDE1C8E75888A6AFBA5* L_0 = __this->___addMethodActions_1;
		ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0* L_1 = (ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0*)il2cpp_codegen_object_new(ServerServiceDefinition_t16402F50122B2FADAD587780C17A2671596DC5D0_il2cpp_TypeInfo_var);
		NullCheck(L_1);
		ServerServiceDefinition__ctor_mF4BC7D215A7F6AECBA8865439697D6F8E2F9E405(L_1, L_0, NULL);
		return L_1;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.ServiceBinderBase::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void ServiceBinderBase__ctor_m62D4E390D2E3AB41EA6202F24AC86E374BAE371C (ServiceBinderBase_tFB0E641375E302B3D9DEC054D9D9BA8197FC104A* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_Multicast(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	il2cpp_array_size_t length = __this->___delegates_13->max_length;
	Delegate_t** delegatesToInvoke = reinterpret_cast<Delegate_t**>(__this->___delegates_13->GetAddressAtUnchecked(0));
	bool retVal = false;
	for (il2cpp_array_size_t i = 0; i < length; i++)
	{
		VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* currentDelegate = reinterpret_cast<VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71*>(delegatesToInvoke[i]);
		typedef bool (*FunctionPointerType) (RuntimeObject*, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8*, const RuntimeMethod*);
		retVal = ((FunctionPointerType)currentDelegate->___invoke_impl_1)((Il2CppObject*)currentDelegate->___method_code_6, ___0_context, reinterpret_cast<RuntimeMethod*>(currentDelegate->___method_3));
	}
	return retVal;
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenInst(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	typedef bool (*FunctionPointerType) (VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___method_ptr_0)(___0_context, method);
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenStatic(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	typedef bool (*FunctionPointerType) (VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___method_ptr_0)(___0_context, method);
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenStaticInvoker(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	return InvokerFuncInvoker1< bool, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* >::Invoke(__this->___method_ptr_0, method, NULL, ___0_context);
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_ClosedStaticInvoker(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	return InvokerFuncInvoker2< bool, RuntimeObject*, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* >::Invoke(__this->___method_ptr_0, method, NULL, __this->___m_target_2, ___0_context);
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenVirtual(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return VirtualFuncInvoker0< bool >::Invoke(il2cpp_codegen_method_get_slot(method), ___0_context);
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenInterface(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return InterfaceFuncInvoker0< bool >::Invoke(il2cpp_codegen_method_get_slot(method), il2cpp_codegen_method_get_declaring_type(method), ___0_context);
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenGenericVirtual(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return GenericVirtualFuncInvoker0< bool >::Invoke(method, ___0_context);
}
bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenGenericInterface(VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method)
{
	NullCheck(___0_context);
	return GenericInterfaceFuncInvoker0< bool >::Invoke(method, ___0_context);
}
// System.Void Grpc.Core.VerifyPeerCallback::.ctor(System.Object,System.IntPtr)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void VerifyPeerCallback__ctor_mD7139772AAD8671507C3037A21D167D8835ADFF0 (VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, RuntimeObject* ___0_object, intptr_t ___1_method, const RuntimeMethod* method) 
{
	__this->___method_ptr_0 = il2cpp_codegen_get_virtual_call_method_pointer((RuntimeMethod*)___1_method);
	__this->___method_3 = ___1_method;
	__this->___m_target_2 = ___0_object;
	Il2CppCodeGenWriteBarrier((void**)(&__this->___m_target_2), (void*)___0_object);
	int parameterCount = il2cpp_codegen_method_parameter_count((RuntimeMethod*)___1_method);
	__this->___method_code_6 = (intptr_t)__this;
	if (MethodIsStatic((RuntimeMethod*)___1_method))
	{
		bool isOpen = parameterCount == 1;
		if (il2cpp_codegen_call_method_via_invoker((RuntimeMethod*)___1_method))
			if (isOpen)
				__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenStaticInvoker;
			else
				__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_ClosedStaticInvoker;
		else
			if (isOpen)
				__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenStatic;
			else
				{
					__this->___invoke_impl_1 = (intptr_t)__this->___method_ptr_0;
					__this->___method_code_6 = (intptr_t)__this->___m_target_2;
				}
	}
	else
	{
		bool isOpen = parameterCount == 0;
		if (isOpen)
		{
			if (__this->___method_is_virtual_12)
			{
				if (il2cpp_codegen_method_is_generic_instance_method((RuntimeMethod*)___1_method))
					if (il2cpp_codegen_method_is_interface_method((RuntimeMethod*)___1_method))
						__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenGenericInterface;
					else
						__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenGenericVirtual;
				else
					if (il2cpp_codegen_method_is_interface_method((RuntimeMethod*)___1_method))
						__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenInterface;
					else
						__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenVirtual;
			}
			else
			{
				__this->___invoke_impl_1 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_OpenInst;
			}
		}
		else
		{
			if (___0_object == NULL)
				il2cpp_codegen_raise_exception(il2cpp_codegen_get_argument_exception(NULL, "Delegate to an instance method cannot have null 'this'."), NULL);
			__this->___invoke_impl_1 = (intptr_t)__this->___method_ptr_0;
			__this->___method_code_6 = (intptr_t)__this->___m_target_2;
		}
	}
	__this->___extra_arg_5 = (intptr_t)&VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358_Multicast;
}
// System.Boolean Grpc.Core.VerifyPeerCallback::Invoke(Grpc.Core.VerifyPeerContext)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool VerifyPeerCallback_Invoke_mB0D359E65DA7D347C8F921B974EE43E944326358 (VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, const RuntimeMethod* method) 
{
	typedef bool (*FunctionPointerType) (RuntimeObject*, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, ___0_context, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
// System.IAsyncResult Grpc.Core.VerifyPeerCallback::BeginInvoke(Grpc.Core.VerifyPeerContext,System.AsyncCallback,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR RuntimeObject* VerifyPeerCallback_BeginInvoke_mAD1A3F440B9AA8AC14352E2BD17F59BD9FAF5A3B (VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* ___0_context, AsyncCallback_t7FEF460CBDCFB9C5FA2EF776984778B9A4145F4C* ___1_callback, RuntimeObject* ___2_object, const RuntimeMethod* method) 
{
	void *__d_args[2] = {0};
	__d_args[0] = ___0_context;
	return (RuntimeObject*)il2cpp_codegen_delegate_begin_invoke((RuntimeDelegate*)__this, __d_args, (RuntimeDelegate*)___1_callback, (RuntimeObject*)___2_object);
}
// System.Boolean Grpc.Core.VerifyPeerCallback::EndInvoke(System.IAsyncResult)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool VerifyPeerCallback_EndInvoke_m124B75EC6466DEF0B488E45A4FEA0E16A7D353D2 (VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* __this, RuntimeObject* ___0_result, const RuntimeMethod* method) 
{
	RuntimeObject *__result = il2cpp_codegen_delegate_end_invoke((Il2CppAsyncResult*) ___0_result, 0);
	return *(bool*)UnBox ((RuntimeObject*)__result);
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.SslCredentials::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SslCredentials__ctor_mB827017C7E77D62BBEACEFE1C7455AB9D619DFB3 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, const RuntimeMethod* method) 
{
	{
		SslCredentials__ctor_mA926FF2EF6211030E5971D99E987190B2DEB2E33(__this, (String_t*)NULL, (KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD*)NULL, (VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71*)NULL, NULL);
		return;
	}
}
// System.Void Grpc.Core.SslCredentials::.ctor(System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SslCredentials__ctor_m04D37EEB31FAE3F06FA02823F1EB7ECD1764AD44 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, String_t* ___0_rootCertificates, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = ___0_rootCertificates;
		SslCredentials__ctor_mA926FF2EF6211030E5971D99E987190B2DEB2E33(__this, L_0, (KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD*)NULL, (VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71*)NULL, NULL);
		return;
	}
}
// System.Void Grpc.Core.SslCredentials::.ctor(System.String,Grpc.Core.KeyCertificatePair)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SslCredentials__ctor_m4BF1F7AE7728CD367E4ED38C723C0AA2A2541D12 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, String_t* ___0_rootCertificates, KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* ___1_keyCertificatePair, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = ___0_rootCertificates;
		KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* L_1 = ___1_keyCertificatePair;
		SslCredentials__ctor_mA926FF2EF6211030E5971D99E987190B2DEB2E33(__this, L_0, L_1, (VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71*)NULL, NULL);
		return;
	}
}
// System.Void Grpc.Core.SslCredentials::.ctor(System.String,Grpc.Core.KeyCertificatePair,Grpc.Core.VerifyPeerCallback)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SslCredentials__ctor_mA926FF2EF6211030E5971D99E987190B2DEB2E33 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, String_t* ___0_rootCertificates, KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* ___1_keyCertificatePair, VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* ___2_verifyPeerCallback, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		il2cpp_codegen_runtime_class_init_inline(ChannelCredentials_t50FD81A0266E8BA60CD2AFB300B0718FDD92D730_il2cpp_TypeInfo_var);
		ChannelCredentials__ctor_m5BAB665085468AD860D966A9B09538F491150849(__this, NULL);
		String_t* L_0 = ___0_rootCertificates;
		__this->___rootCertificates_2 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___rootCertificates_2), (void*)L_0);
		KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* L_1 = ___1_keyCertificatePair;
		__this->___keyCertificatePair_3 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___keyCertificatePair_3), (void*)L_1);
		VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* L_2 = ___2_verifyPeerCallback;
		__this->___verifyPeerCallback_4 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___verifyPeerCallback_4), (void*)L_2);
		return;
	}
}
// System.String Grpc.Core.SslCredentials::get_RootCertificates()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* SslCredentials_get_RootCertificates_m69CD45CADC386DF91AC3D04631CD7806848FEFC4 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___rootCertificates_2;
		return L_0;
	}
}
// Grpc.Core.KeyCertificatePair Grpc.Core.SslCredentials::get_KeyCertificatePair()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* SslCredentials_get_KeyCertificatePair_m133B0D6BDBE22F47D7BC2C710923E0B90168573C (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, const RuntimeMethod* method) 
{
	{
		KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* L_0 = __this->___keyCertificatePair_3;
		return L_0;
	}
}
// System.Void Grpc.Core.SslCredentials::InternalPopulateConfiguration(Grpc.Core.ChannelCredentialsConfiguratorBase,System.Object)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void SslCredentials_InternalPopulateConfiguration_m820ACCBE5BB4C428CD2FCBC63B970032314AD2F0 (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E* ___0_configurator, RuntimeObject* ___1_state, const RuntimeMethod* method) 
{
	{
		ChannelCredentialsConfiguratorBase_t421C1250ECD3F60193A113B90F54D2B92A5BCC3E* L_0 = ___0_configurator;
		RuntimeObject* L_1 = ___1_state;
		String_t* L_2 = __this->___rootCertificates_2;
		KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD* L_3 = __this->___keyCertificatePair_3;
		VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* L_4 = __this->___verifyPeerCallback_4;
		NullCheck(L_0);
		VirtualActionInvoker4< RuntimeObject*, String_t*, KeyCertificatePair_t0B2C1F1D3BB202CA4C275375C6519E76AD864ABD*, VerifyPeerCallback_t360DD66F714C6D72C115A73C1287713EE8D60D71* >::Invoke(5 /* System.Void Grpc.Core.ChannelCredentialsConfiguratorBase::SetSslCredentials(System.Object,System.String,Grpc.Core.KeyCertificatePair,Grpc.Core.VerifyPeerCallback) */, L_0, L_1, L_2, L_3, L_4);
		return;
	}
}
// System.Boolean Grpc.Core.SslCredentials::get_IsComposable()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR bool SslCredentials_get_IsComposable_m2710E0A02EFEBAFFBF8976E33A386FC954A57EDD (SslCredentials_t70CEB9A0802464F4BB5240810E249248CC1C1CD3* __this, const RuntimeMethod* method) 
{
	{
		return (bool)1;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif


// Conversion methods for marshalling of: Grpc.Core.Status
IL2CPP_EXTERN_C void Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshal_pinvoke(const Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642& unmarshaled, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_pinvoke& marshaled)
{
	Exception_t* ___U3CDebugExceptionU3Ek__BackingField_4Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field '<DebugException>k__BackingField' of type 'Status': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___U3CDebugExceptionU3Ek__BackingField_4Exception, NULL);
}
IL2CPP_EXTERN_C void Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshal_pinvoke_back(const Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_pinvoke& marshaled, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642& unmarshaled)
{
	Exception_t* ___U3CDebugExceptionU3Ek__BackingField_4Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field '<DebugException>k__BackingField' of type 'Status': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___U3CDebugExceptionU3Ek__BackingField_4Exception, NULL);
}
// Conversion method for clean up from marshalling of: Grpc.Core.Status
IL2CPP_EXTERN_C void Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshal_pinvoke_cleanup(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_pinvoke& marshaled)
{
}


// Conversion methods for marshalling of: Grpc.Core.Status
IL2CPP_EXTERN_C void Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshal_com(const Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642& unmarshaled, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_com& marshaled)
{
	Exception_t* ___U3CDebugExceptionU3Ek__BackingField_4Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field '<DebugException>k__BackingField' of type 'Status': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___U3CDebugExceptionU3Ek__BackingField_4Exception, NULL);
}
IL2CPP_EXTERN_C void Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshal_com_back(const Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_com& marshaled, Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642& unmarshaled)
{
	Exception_t* ___U3CDebugExceptionU3Ek__BackingField_4Exception = il2cpp_codegen_get_marshal_directive_exception("Cannot marshal field '<DebugException>k__BackingField' of type 'Status': Reference type field marshaling is not supported.");
	IL2CPP_RAISE_MANAGED_EXCEPTION(___U3CDebugExceptionU3Ek__BackingField_4Exception, NULL);
}
// Conversion method for clean up from marshalling of: Grpc.Core.Status
IL2CPP_EXTERN_C void Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshal_com_cleanup(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_marshaled_com& marshaled)
{
}
// System.Void Grpc.Core.Status::.ctor(Grpc.Core.StatusCode,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Status__ctor_mC9E29F2BD60C9C3C1A3AD4A834052ABD9CBC89FF (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, int32_t ___0_statusCode, String_t* ___1_detail, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = ___0_statusCode;
		String_t* L_1 = ___1_detail;
		Status__ctor_m212E9F49C1AE80241D1C447DC18E3C09E3641D4E(__this, L_0, L_1, (Exception_t*)NULL, NULL);
		return;
	}
}
IL2CPP_EXTERN_C  void Status__ctor_mC9E29F2BD60C9C3C1A3AD4A834052ABD9CBC89FF_AdjustorThunk (RuntimeObject* __this, int32_t ___0_statusCode, String_t* ___1_detail, const RuntimeMethod* method)
{
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642*>(__this + _offset);
	Status__ctor_mC9E29F2BD60C9C3C1A3AD4A834052ABD9CBC89FF(_thisAdjusted, ___0_statusCode, ___1_detail, method);
}
// System.Void Grpc.Core.Status::.ctor(Grpc.Core.StatusCode,System.String,System.Exception)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Status__ctor_m212E9F49C1AE80241D1C447DC18E3C09E3641D4E (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, int32_t ___0_statusCode, String_t* ___1_detail, Exception_t* ___2_debugException, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = ___0_statusCode;
		__this->___U3CStatusCodeU3Ek__BackingField_2 = L_0;
		String_t* L_1 = ___1_detail;
		__this->___U3CDetailU3Ek__BackingField_3 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___U3CDetailU3Ek__BackingField_3), (void*)L_1);
		Exception_t* L_2 = ___2_debugException;
		__this->___U3CDebugExceptionU3Ek__BackingField_4 = L_2;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___U3CDebugExceptionU3Ek__BackingField_4), (void*)L_2);
		return;
	}
}
IL2CPP_EXTERN_C  void Status__ctor_m212E9F49C1AE80241D1C447DC18E3C09E3641D4E_AdjustorThunk (RuntimeObject* __this, int32_t ___0_statusCode, String_t* ___1_detail, Exception_t* ___2_debugException, const RuntimeMethod* method)
{
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642*>(__this + _offset);
	Status__ctor_m212E9F49C1AE80241D1C447DC18E3C09E3641D4E(_thisAdjusted, ___0_statusCode, ___1_detail, ___2_debugException, method);
}
// Grpc.Core.StatusCode Grpc.Core.Status::get_StatusCode()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386 (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->___U3CStatusCodeU3Ek__BackingField_2;
		return L_0;
	}
}
IL2CPP_EXTERN_C  int32_t Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642*>(__this + _offset);
	int32_t _returnValue;
	_returnValue = Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386_inline(_thisAdjusted, method);
	return _returnValue;
}
// System.String Grpc.Core.Status::get_Detail()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Status_get_Detail_m27B09DC9C3459A6DE0461139E6F7012EA86C49BB (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___U3CDetailU3Ek__BackingField_3;
		return L_0;
	}
}
IL2CPP_EXTERN_C  String_t* Status_get_Detail_m27B09DC9C3459A6DE0461139E6F7012EA86C49BB_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642*>(__this + _offset);
	String_t* _returnValue;
	_returnValue = Status_get_Detail_m27B09DC9C3459A6DE0461139E6F7012EA86C49BB_inline(_thisAdjusted, method);
	return _returnValue;
}
// System.Exception Grpc.Core.Status::get_DebugException()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR Exception_t* Status_get_DebugException_m434DF404A6BDDD4910C19272ED5516F95500A359 (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) 
{
	{
		Exception_t* L_0 = __this->___U3CDebugExceptionU3Ek__BackingField_4;
		return L_0;
	}
}
IL2CPP_EXTERN_C  Exception_t* Status_get_DebugException_m434DF404A6BDDD4910C19272ED5516F95500A359_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642*>(__this + _offset);
	Exception_t* _returnValue;
	_returnValue = Status_get_DebugException_m434DF404A6BDDD4910C19272ED5516F95500A359_inline(_thisAdjusted, method);
	return _returnValue;
}
// System.String Grpc.Core.Status::ToString()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* Status_ToString_m85A7C885F4E22BC641454E680EB9AB02A8A79036 (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&StatusCode_t96CF7C2EFE67B9B0DCEF94E8B24F704141287375_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralCC5275F31A11D4AC091381249B0215D81AB03CF9);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralEFDA3D70B0D50B492DB756FABC3B4A4E00B0F8AF);
		s_Il2CppMethodInitialized = true;
	}
	{
		Exception_t* L_0;
		L_0 = Status_get_DebugException_m434DF404A6BDDD4910C19272ED5516F95500A359_inline(__this, NULL);
		if (!L_0)
		{
			goto IL_002a;
		}
	}
	{
		int32_t L_1;
		L_1 = Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386_inline(__this, NULL);
		int32_t L_2 = L_1;
		RuntimeObject* L_3 = Box(StatusCode_t96CF7C2EFE67B9B0DCEF94E8B24F704141287375_il2cpp_TypeInfo_var, &L_2);
		String_t* L_4;
		L_4 = Status_get_Detail_m27B09DC9C3459A6DE0461139E6F7012EA86C49BB_inline(__this, NULL);
		Exception_t* L_5;
		L_5 = Status_get_DebugException_m434DF404A6BDDD4910C19272ED5516F95500A359_inline(__this, NULL);
		String_t* L_6;
		L_6 = String_Format_mA0534D6E2AE4D67A6BD8D45B3321323930EB930C(_stringLiteralEFDA3D70B0D50B492DB756FABC3B4A4E00B0F8AF, L_3, L_4, L_5, NULL);
		return L_6;
	}

IL_002a:
	{
		int32_t L_7;
		L_7 = Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386_inline(__this, NULL);
		int32_t L_8 = L_7;
		RuntimeObject* L_9 = Box(StatusCode_t96CF7C2EFE67B9B0DCEF94E8B24F704141287375_il2cpp_TypeInfo_var, &L_8);
		String_t* L_10;
		L_10 = Status_get_Detail_m27B09DC9C3459A6DE0461139E6F7012EA86C49BB_inline(__this, NULL);
		String_t* L_11;
		L_11 = String_Format_mFB7DA489BD99F4670881FF50EC017BFB0A5C0987(_stringLiteralCC5275F31A11D4AC091381249B0215D81AB03CF9, L_9, L_10, NULL);
		return L_11;
	}
}
IL2CPP_EXTERN_C  String_t* Status_ToString_m85A7C885F4E22BC641454E680EB9AB02A8A79036_AdjustorThunk (RuntimeObject* __this, const RuntimeMethod* method)
{
	Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* _thisAdjusted;
	int32_t _offset = 1;
	_thisAdjusted = reinterpret_cast<Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642*>(__this + _offset);
	String_t* _returnValue;
	_returnValue = Status_ToString_m85A7C885F4E22BC641454E680EB9AB02A8A79036(_thisAdjusted, method);
	return _returnValue;
}
// System.Void Grpc.Core.Status::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Status__cctor_m49D0DABA9FAC8C438C56A7314ABF7423FF3B6784 (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709);
		s_Il2CppMethodInitialized = true;
	}
	{
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_0;
		memset((&L_0), 0, sizeof(L_0));
		Status__ctor_mC9E29F2BD60C9C3C1A3AD4A834052ABD9CBC89FF((&L_0), 0, _stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709, /*hidden argument*/NULL);
		((Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_StaticFields*)il2cpp_codegen_static_fields_for(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var))->___DefaultSuccess_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)&(((&((Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_StaticFields*)il2cpp_codegen_static_fields_for(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var))->___DefaultSuccess_0))->___U3CDetailU3Ek__BackingField_3), (void*)NULL);
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&(((&((Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_StaticFields*)il2cpp_codegen_static_fields_for(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var))->___DefaultSuccess_0))->___U3CDebugExceptionU3Ek__BackingField_4), (void*)NULL);
		#endif
		Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 L_1;
		memset((&L_1), 0, sizeof(L_1));
		Status__ctor_mC9E29F2BD60C9C3C1A3AD4A834052ABD9CBC89FF((&L_1), 1, _stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709, /*hidden argument*/NULL);
		((Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_StaticFields*)il2cpp_codegen_static_fields_for(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var))->___DefaultCancelled_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)&(((&((Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_StaticFields*)il2cpp_codegen_static_fields_for(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var))->___DefaultCancelled_1))->___U3CDetailU3Ek__BackingField_3), (void*)NULL);
		#if IL2CPP_ENABLE_STRICT_WRITE_BARRIERS
		Il2CppCodeGenWriteBarrier((void**)&(((&((Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_StaticFields*)il2cpp_codegen_static_fields_for(Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642_il2cpp_TypeInfo_var))->___DefaultCancelled_1))->___U3CDebugExceptionU3Ek__BackingField_4), (void*)NULL);
		#endif
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.VerifyPeerContext::.ctor(System.String,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void VerifyPeerContext__ctor_m0E131359E089C8FDB60D67D4466CA8FAB4CE5982 (VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* __this, String_t* ___0_targetName, String_t* ___1_peerPem, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		String_t* L_0 = ___0_targetName;
		__this->___U3CTargetNameU3Ek__BackingField_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___U3CTargetNameU3Ek__BackingField_0), (void*)L_0);
		String_t* L_1 = ___1_peerPem;
		__this->___U3CPeerPemU3Ek__BackingField_1 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___U3CPeerPemU3Ek__BackingField_1), (void*)L_1);
		return;
	}
}
// System.String Grpc.Core.VerifyPeerContext::get_TargetName()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* VerifyPeerContext_get_TargetName_mC41C04803DBC1B3C32A71E4B053C18E648F78447 (VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___U3CTargetNameU3Ek__BackingField_0;
		return L_0;
	}
}
// System.String Grpc.Core.VerifyPeerContext::get_PeerPem()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* VerifyPeerContext_get_PeerPem_mD7F7CA3D2D7130E33AA8592F115C28C4E939769E (VerifyPeerContext_t08DE966AD4564DE4B35C575B797ACAC385B683C8* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___U3CPeerPemU3Ek__BackingField_1;
		return L_0;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.WriteOptions::.ctor(Grpc.Core.WriteFlags)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void WriteOptions__ctor_m189F6B667BC45FC77CF649CC21B794DCC3F83FF5 (WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* __this, int32_t ___0_flags, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		int32_t L_0 = ___0_flags;
		__this->___flags_1 = L_0;
		return;
	}
}
// Grpc.Core.WriteFlags Grpc.Core.WriteOptions::get_Flags()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR int32_t WriteOptions_get_Flags_m38CC27767B0D73AE83AC0479B28F5E0CF245FD80 (WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->___flags_1;
		return L_0;
	}
}
// System.Void Grpc.Core.WriteOptions::.cctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void WriteOptions__cctor_m5501DEF4BB89AF55FFE8383C4FEE8875672CD68D (const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* L_0 = (WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050*)il2cpp_codegen_object_new(WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_il2cpp_TypeInfo_var);
		NullCheck(L_0);
		WriteOptions__ctor_m189F6B667BC45FC77CF649CC21B794DCC3F83FF5(L_0, 0, NULL);
		((WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_StaticFields*)il2cpp_codegen_static_fields_for(WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_il2cpp_TypeInfo_var))->___Default_0 = L_0;
		Il2CppCodeGenWriteBarrier((void**)(&((WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_StaticFields*)il2cpp_codegen_static_fields_for(WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050_il2cpp_TypeInfo_var))->___Default_0), (void*)L_0);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Utils.GrpcPreconditions::CheckArgument(System.Boolean)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void GrpcPreconditions_CheckArgument_m2B157D8C7C01045ECCABED1F0A51A889C9759D34 (bool ___0_condition, const RuntimeMethod* method) 
{
	{
		bool L_0 = ___0_condition;
		if (L_0)
		{
			goto IL_0009;
		}
	}
	{
		ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263* L_1 = (ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263_il2cpp_TypeInfo_var)));
		NullCheck(L_1);
		ArgumentException__ctor_m34A925BA55EC4CE4253404E363B5F6A53EB51CA3(L_1, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_1, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&GrpcPreconditions_CheckArgument_m2B157D8C7C01045ECCABED1F0A51A889C9759D34_RuntimeMethod_var)));
	}

IL_0009:
	{
		return;
	}
}
// System.Void Grpc.Core.Utils.GrpcPreconditions::CheckArgument(System.Boolean,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497 (bool ___0_condition, String_t* ___1_errorMessage, const RuntimeMethod* method) 
{
	{
		bool L_0 = ___0_condition;
		if (L_0)
		{
			goto IL_000a;
		}
	}
	{
		String_t* L_1 = ___1_errorMessage;
		ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263* L_2 = (ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&ArgumentException_tAD90411542A20A9C72D5CDA3A84181D8B947A263_il2cpp_TypeInfo_var)));
		NullCheck(L_2);
		ArgumentException__ctor_m026938A67AF9D36BB7ED27F80425D7194B514465(L_2, L_1, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_2, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&GrpcPreconditions_CheckArgument_mF79300A09AB285B357FF977951E4797A42EDB497_RuntimeMethod_var)));
	}

IL_000a:
	{
		return;
	}
}
// System.Void Grpc.Core.Utils.GrpcPreconditions::CheckState(System.Boolean)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void GrpcPreconditions_CheckState_m8B3B2F0B0AE8E3A42D3252666E1E51C402CE81E9 (bool ___0_condition, const RuntimeMethod* method) 
{
	{
		bool L_0 = ___0_condition;
		if (L_0)
		{
			goto IL_0009;
		}
	}
	{
		InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB* L_1 = (InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB_il2cpp_TypeInfo_var)));
		NullCheck(L_1);
		InvalidOperationException__ctor_m1BE9BD198B904AA1D94F4B10DA88077DFD44B7A5(L_1, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_1, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&GrpcPreconditions_CheckState_m8B3B2F0B0AE8E3A42D3252666E1E51C402CE81E9_RuntimeMethod_var)));
	}

IL_0009:
	{
		return;
	}
}
// System.Void Grpc.Core.Utils.GrpcPreconditions::CheckState(System.Boolean,System.String)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void GrpcPreconditions_CheckState_mCDE04614CE69128AF99EE030B5F83D386FDEC375 (bool ___0_condition, String_t* ___1_errorMessage, const RuntimeMethod* method) 
{
	{
		bool L_0 = ___0_condition;
		if (L_0)
		{
			goto IL_000a;
		}
	}
	{
		String_t* L_1 = ___1_errorMessage;
		InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB* L_2 = (InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB*)il2cpp_codegen_object_new(((RuntimeClass*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&InvalidOperationException_t5DDE4D49B7405FAAB1E4576F4715A42A3FAD4BAB_il2cpp_TypeInfo_var)));
		NullCheck(L_2);
		InvalidOperationException__ctor_mE4CB6F4712AB6D99A2358FBAE2E052B3EE976162(L_2, L_1, NULL);
		IL2CPP_RAISE_MANAGED_EXCEPTION(L_2, ((RuntimeMethod*)il2cpp_codegen_initialize_runtime_metadata_inline((uintptr_t*)&GrpcPreconditions_CheckState_mCDE04614CE69128AF99EE030B5F83D386FDEC375_RuntimeMethod_var)));
	}

IL_000a:
	{
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.String Grpc.Core.Api.Utils.EncodingExtensions::GetString(System.Text.Encoding,System.Byte*,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* EncodingExtensions_GetString_mF5E63092112C64E8D48B96690474700EBAFFD672 (Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___0_encoding, uint8_t* ___1_source, int32_t ___2_byteCount, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709);
		s_Il2CppMethodInitialized = true;
	}
	int32_t V_0 = 0;
	Il2CppChar* V_1 = NULL;
	String_t* V_2 = NULL;
	String_t* G_B4_0 = NULL;
	String_t* G_B3_0 = NULL;
	{
		int32_t L_0 = ___2_byteCount;
		if (L_0)
		{
			goto IL_0009;
		}
	}
	{
		return _stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709;
	}

IL_0009:
	{
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_1 = ___0_encoding;
		uint8_t* L_2 = ___1_source;
		int32_t L_3 = ___2_byteCount;
		NullCheck(L_1);
		int32_t L_4;
		L_4 = VirtualFuncInvoker2< int32_t, uint8_t*, int32_t >::Invoke(22 /* System.Int32 System.Text.Encoding::GetCharCount(System.Byte*,System.Int32) */, L_1, L_2, L_3);
		V_0 = L_4;
		int32_t L_5 = V_0;
		String_t* L_6;
		L_6 = String_CreateString_mAA0705B41B390BDB42F67894B9B67C956814C71B(NULL, 0, L_5, NULL);
		String_t* L_7 = L_6;
		V_2 = L_7;
		String_t* L_8 = V_2;
		V_1 = (Il2CppChar*)((uintptr_t)L_8);
		Il2CppChar* L_9 = V_1;
		G_B3_0 = L_7;
		if (!L_9)
		{
			G_B4_0 = L_7;
			goto IL_0029;
		}
	}
	{
		Il2CppChar* L_10 = V_1;
		int32_t L_11;
		L_11 = RuntimeHelpers_get_OffsetToStringData_m90A5D27EF88BE9432BF7093B7D7E7A0ACB0A8FBD(NULL);
		V_1 = ((Il2CppChar*)il2cpp_codegen_add((intptr_t)L_10, L_11));
		G_B4_0 = G_B3_0;
	}

IL_0029:
	{
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_12 = ___0_encoding;
		uint8_t* L_13 = ___1_source;
		int32_t L_14 = ___2_byteCount;
		Il2CppChar* L_15 = V_1;
		int32_t L_16 = V_0;
		NullCheck(L_12);
		int32_t L_17;
		L_17 = VirtualFuncInvoker4< int32_t, uint8_t*, int32_t, Il2CppChar*, int32_t >::Invoke(26 /* System.Int32 System.Text.Encoding::GetChars(System.Byte*,System.Int32,System.Char*,System.Int32) */, L_12, L_13, L_14, L_15, L_16);
		V_2 = (String_t*)NULL;
		return G_B4_0;
	}
}
// System.String Grpc.Core.Api.Utils.EncodingExtensions::GetString(System.Text.Encoding,System.IntPtr,System.Int32)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR String_t* EncodingExtensions_GetString_m5290CA29398B11EA0507EEEBE965E7E78137EEFF (Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___0_encoding, intptr_t ___1_ptr, int32_t ___2_len, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709);
		s_Il2CppMethodInitialized = true;
	}
	{
		int32_t L_0 = ___2_len;
		if (!L_0)
		{
			goto IL_0012;
		}
	}
	{
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_1 = ___0_encoding;
		void* L_2;
		L_2 = IntPtr_ToPointer_m1A0612EED3A1C8B8850BE2943CFC42523064B4F6_inline((&___1_ptr), NULL);
		int32_t L_3 = ___2_len;
		String_t* L_4;
		L_4 = EncodingExtensions_GetString_mF5E63092112C64E8D48B96690474700EBAFFD672(L_1, (uint8_t*)L_2, L_3, NULL);
		return L_4;
	}

IL_0012:
	{
		return _stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.CallInvokerExtensions::Intercept(Grpc.Core.CallInvoker,Grpc.Core.Interceptors.Interceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* CallInvokerExtensions_Intercept_m415B574E557536E895A7DAC8AE5B3B14026B135A (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* ___1_interceptor, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = ___0_invoker;
		Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* L_1 = ___1_interceptor;
		InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4* L_2 = (InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4*)il2cpp_codegen_object_new(InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		InterceptingCallInvoker__ctor_m32BAFD6EB2940991FD48CB27B3FAEFD82CF63C01(L_2, L_0, L_1, NULL);
		return L_2;
	}
}
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.CallInvokerExtensions::Intercept(Grpc.Core.CallInvoker,Grpc.Core.Interceptors.Interceptor[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* CallInvokerExtensions_Intercept_m5D4276B12009D07A5A34D25827644546264A071B (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* ___1_interceptors, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&Enumerable_Reverse_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m1C1BF18516E49D40D7DBDB84100A0B8168E2B4BA_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisInterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2_m54DCBCEF26FDD11320936B2E59AE28F2E7CAC4CC_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IDisposable_t030E0496B4E0E4E4F086825007979AF51F7248C5_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IEnumerable_1_t37CBE1A5D68AA0B27E8B20742699245769A385E0_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IEnumerator_1_t5F4BEB00470960A0E76DDE9DAA1DF5168331CB41_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&IEnumerator_t7B609C2FFA6EB5167D9C62A0C32A21DE2F666DAA_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral7C19A3E78437A0AD89B0D16EBF9C74FEDCCBB8AC);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralE064A593217F8D9C315BF354EAAD770AB3C8246C);
		s_Il2CppMethodInitialized = true;
	}
	RuntimeObject* V_0 = NULL;
	Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* V_1 = NULL;
	{
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = ___0_invoker;
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC(L_0, _stringLiteralE064A593217F8D9C315BF354EAAD770AB3C8246C, GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC_RuntimeMethod_var);
		InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* L_2 = ___1_interceptors;
		InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* L_3;
		L_3 = GrpcPreconditions_CheckNotNull_TisInterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2_m54DCBCEF26FDD11320936B2E59AE28F2E7CAC4CC(L_2, _stringLiteral7C19A3E78437A0AD89B0D16EBF9C74FEDCCBB8AC, GrpcPreconditions_CheckNotNull_TisInterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2_m54DCBCEF26FDD11320936B2E59AE28F2E7CAC4CC_RuntimeMethod_var);
		InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* L_4 = ___1_interceptors;
		RuntimeObject* L_5;
		L_5 = Enumerable_Reverse_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m1C1BF18516E49D40D7DBDB84100A0B8168E2B4BA((RuntimeObject*)L_4, Enumerable_Reverse_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m1C1BF18516E49D40D7DBDB84100A0B8168E2B4BA_RuntimeMethod_var);
		NullCheck(L_5);
		RuntimeObject* L_6;
		L_6 = InterfaceFuncInvoker0< RuntimeObject* >::Invoke(0 /* System.Collections.Generic.IEnumerator`1<T> System.Collections.Generic.IEnumerable`1<Grpc.Core.Interceptors.Interceptor>::GetEnumerator() */, IEnumerable_1_t37CBE1A5D68AA0B27E8B20742699245769A385E0_il2cpp_TypeInfo_var, L_5);
		V_0 = L_6;
	}
	{
		auto __finallyBlock = il2cpp::utils::Finally([&]
		{

FINALLY_0040:
			{// begin finally (depth: 1)
				{
					RuntimeObject* L_7 = V_0;
					if (!L_7)
					{
						goto IL_0049;
					}
				}
				{
					RuntimeObject* L_8 = V_0;
					NullCheck(L_8);
					InterfaceActionInvoker0::Invoke(0 /* System.Void System.IDisposable::Dispose() */, IDisposable_t030E0496B4E0E4E4F086825007979AF51F7248C5_il2cpp_TypeInfo_var, L_8);
				}

IL_0049:
				{
					return;
				}
			}// end finally (depth: 1)
		});
		try
		{// begin try (depth: 1)
			{
				goto IL_0036_1;
			}

IL_0026_1:
			{
				RuntimeObject* L_9 = V_0;
				NullCheck(L_9);
				Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* L_10;
				L_10 = InterfaceFuncInvoker0< Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* >::Invoke(0 /* T System.Collections.Generic.IEnumerator`1<Grpc.Core.Interceptors.Interceptor>::get_Current() */, IEnumerator_1_t5F4BEB00470960A0E76DDE9DAA1DF5168331CB41_il2cpp_TypeInfo_var, L_9);
				V_1 = L_10;
				CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_11 = ___0_invoker;
				Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* L_12 = V_1;
				CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_13;
				L_13 = CallInvokerExtensions_Intercept_m415B574E557536E895A7DAC8AE5B3B14026B135A(L_11, L_12, NULL);
				___0_invoker = L_13;
			}

IL_0036_1:
			{
				RuntimeObject* L_14 = V_0;
				NullCheck(L_14);
				bool L_15;
				L_15 = InterfaceFuncInvoker0< bool >::Invoke(0 /* System.Boolean System.Collections.IEnumerator::MoveNext() */, IEnumerator_t7B609C2FFA6EB5167D9C62A0C32A21DE2F666DAA_il2cpp_TypeInfo_var, L_14);
				if (L_15)
				{
					goto IL_0026_1;
				}
			}
			{
				goto IL_004a;
			}
		}// end try (depth: 1)
		catch(Il2CppExceptionWrapper& e)
		{
			__finallyBlock.StoreException(e.ex);
		}
	}

IL_004a:
	{
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_16 = ___0_invoker;
		return L_16;
	}
}
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.CallInvokerExtensions::Intercept(Grpc.Core.CallInvoker,System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* CallInvokerExtensions_Intercept_mE077F5FBF93ED074216668E3CD469F4F260C300F (CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* ___1_interceptor, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4_il2cpp_TypeInfo_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975_il2cpp_TypeInfo_var);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = ___0_invoker;
		Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* L_1 = ___1_interceptor;
		MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975* L_2 = (MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975*)il2cpp_codegen_object_new(MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975_il2cpp_TypeInfo_var);
		NullCheck(L_2);
		MetadataInterceptor__ctor_mE4EAD6608BAD8917BB3873CF2245A67A2F9DEAA3(L_2, L_1, NULL);
		InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4* L_3 = (InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4*)il2cpp_codegen_object_new(InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4_il2cpp_TypeInfo_var);
		NullCheck(L_3);
		InterceptingCallInvoker__ctor_m32BAFD6EB2940991FD48CB27B3FAEFD82CF63C01(L_3, L_0, L_2, NULL);
		return L_3;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Interceptors.CallInvokerExtensions/MetadataInterceptor::.ctor(System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void MetadataInterceptor__ctor_mE4EAD6608BAD8917BB3873CF2245A67A2F9DEAA3 (MetadataInterceptor_tBF39268551AD179FEA17CBE607BB2FF3142C2975* __this, Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* ___0_interceptor, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisFunc_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B_m5E04E5E300A4C3AD62F603401B642AB7C7656305_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral29D01ED29B3E08D159739A60EE14FFC25AC4F8FE);
		s_Il2CppMethodInitialized = true;
	}
	{
		Interceptor__ctor_m0A48ED17B15161D356A95F546D1AADB995725169(__this, NULL);
		Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* L_0 = ___0_interceptor;
		Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisFunc_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B_m5E04E5E300A4C3AD62F603401B642AB7C7656305(L_0, _stringLiteral29D01ED29B3E08D159739A60EE14FFC25AC4F8FE, GrpcPreconditions_CheckNotNull_TisFunc_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B_m5E04E5E300A4C3AD62F603401B642AB7C7656305_RuntimeMethod_var);
		__this->___interceptor_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___interceptor_0), (void*)L_1);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.ChannelExtensions::Intercept(Grpc.Core.ChannelBase,Grpc.Core.Interceptors.Interceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ChannelExtensions_Intercept_mD351A9BB4593948003C8ADC83889D54BD6B32960 (ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* ___0_channel, Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* ___1_interceptor, const RuntimeMethod* method) 
{
	{
		ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* L_0 = ___0_channel;
		NullCheck(L_0);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_1;
		L_1 = VirtualFuncInvoker0< CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* >::Invoke(4 /* Grpc.Core.CallInvoker Grpc.Core.ChannelBase::CreateCallInvoker() */, L_0);
		Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* L_2 = ___1_interceptor;
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_3;
		L_3 = CallInvokerExtensions_Intercept_m415B574E557536E895A7DAC8AE5B3B14026B135A(L_1, L_2, NULL);
		return L_3;
	}
}
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.ChannelExtensions::Intercept(Grpc.Core.ChannelBase,Grpc.Core.Interceptors.Interceptor[])
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ChannelExtensions_Intercept_mBD98215FBEB9ED9656625E335BF9B8C256BD42CA (ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* ___0_channel, InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* ___1_interceptors, const RuntimeMethod* method) 
{
	{
		ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* L_0 = ___0_channel;
		NullCheck(L_0);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_1;
		L_1 = VirtualFuncInvoker0< CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* >::Invoke(4 /* Grpc.Core.CallInvoker Grpc.Core.ChannelBase::CreateCallInvoker() */, L_0);
		InterceptorU5BU5D_t07875A3441F35177EBF82ABDBEE497D039B85FB2* L_2 = ___1_interceptors;
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_3;
		L_3 = CallInvokerExtensions_Intercept_m5D4276B12009D07A5A34D25827644546264A071B(L_1, L_2, NULL);
		return L_3;
	}
}
// Grpc.Core.CallInvoker Grpc.Core.Interceptors.ChannelExtensions::Intercept(Grpc.Core.ChannelBase,System.Func`2<Grpc.Core.Metadata,Grpc.Core.Metadata>)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ChannelExtensions_Intercept_m8B04D0370830D3AD4A096D7A7A2945E6F861E4CE (ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* ___0_channel, Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* ___1_interceptor, const RuntimeMethod* method) 
{
	{
		ChannelBase_tD276AC64DE64B8C1BFD85654A280D93E69DA40E5* L_0 = ___0_channel;
		NullCheck(L_0);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_1;
		L_1 = VirtualFuncInvoker0< CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* >::Invoke(4 /* Grpc.Core.CallInvoker Grpc.Core.ChannelBase::CreateCallInvoker() */, L_0);
		Func_2_t5088E922DE1F7A47A154DED6C43EFFC8FC63893B* L_2 = ___1_interceptor;
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_3;
		L_3 = CallInvokerExtensions_Intercept_mE077F5FBF93ED074216668E3CD469F4F260C300F(L_1, L_2, NULL);
		return L_3;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Interceptors.InterceptingCallInvoker::.ctor(Grpc.Core.CallInvoker,Grpc.Core.Interceptors.Interceptor)
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void InterceptingCallInvoker__ctor_m32BAFD6EB2940991FD48CB27B3FAEFD82CF63C01 (InterceptingCallInvoker_t00C10B004D65E49D103D11488F599C3AAC1E4EE4* __this, CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* ___0_invoker, Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* ___1_interceptor, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&GrpcPreconditions_CheckNotNull_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m4E082761E5C0C7589B1EA55BB46E22E68FDA6B54_RuntimeMethod_var);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteral29D01ED29B3E08D159739A60EE14FFC25AC4F8FE);
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralE064A593217F8D9C315BF354EAAD770AB3C8246C);
		s_Il2CppMethodInitialized = true;
	}
	{
		CallInvoker__ctor_mFFDA4B00606F063F1EAC9DBBCDDECD9F82705E6B(__this, NULL);
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_0 = ___0_invoker;
		CallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF* L_1;
		L_1 = GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC(L_0, _stringLiteralE064A593217F8D9C315BF354EAAD770AB3C8246C, GrpcPreconditions_CheckNotNull_TisCallInvoker_t4CBA795695863F40520DEBAEBA67550CC2D854AF_m34A183CD5669C42A9D59BBA8BF04AE343959CEAC_RuntimeMethod_var);
		__this->___invoker_0 = L_1;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___invoker_0), (void*)L_1);
		Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* L_2 = ___1_interceptor;
		Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* L_3;
		L_3 = GrpcPreconditions_CheckNotNull_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m4E082761E5C0C7589B1EA55BB46E22E68FDA6B54(L_2, _stringLiteral29D01ED29B3E08D159739A60EE14FFC25AC4F8FE, GrpcPreconditions_CheckNotNull_TisInterceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0_m4E082761E5C0C7589B1EA55BB46E22E68FDA6B54_RuntimeMethod_var);
		__this->___interceptor_1 = L_3;
		Il2CppCodeGenWriteBarrier((void**)(&__this->___interceptor_1), (void*)L_3);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Interceptors.Interceptor::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void Interceptor__ctor_m0A48ED17B15161D356A95F546D1AADB995725169 (Interceptor_t196108E40F6432CECD523B69C32C3D5CE21ABEE0* __this, const RuntimeMethod* method) 
{
	{
		Object__ctor_mE837C6B9FA8C6D5D109F4B2EC885D79919AC0EA2(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"
#pragma clang diagnostic ignored "-Wunused-variable"
#endif
// System.Void Grpc.Core.Internal.UnimplementedCallInvoker::.ctor()
IL2CPP_EXTERN_C IL2CPP_METHOD_ATTR void UnimplementedCallInvoker__ctor_m56CA5FABA47729295B56022F92B907D32DBFA0AD (UnimplementedCallInvoker_tAE6E88414834E7998AFF7D8868FC4EDF12665435* __this, const RuntimeMethod* method) 
{
	{
		CallInvoker__ctor_mFFDA4B00606F063F1EAC9DBBCDDECD9F82705E6B(__this, NULL);
		return;
	}
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void Action_Invoke_m7126A54DACA72B845424072887B5F3A51FC3808E_inline (Action_tD00B0A84D7945E50C2DFFC28EFEE6ED44ED2AD07* __this, const RuntimeMethod* method) 
{
	typedef void (*FunctionPointerType) (RuntimeObject*, const RuntimeMethod*);
	((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* CallOptions_get_Headers_mC83DA74FDC549907063A89E8E3589E244EE244BB_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		Metadata_tDFD6D82DBE7D557D3971013D7803AA27C8514F06* L_0 = __this->___headers_0;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC CallOptions_get_Deadline_m2B9B9FB43004CBA8DE9B7BFE125991B4FB24EEDA_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		Nullable_1_tEADC262F7F8B8BC4CC0A003DBDD3CA7C1B63F9AC L_0 = __this->___deadline_1;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED CallOptions_get_CancellationToken_m26B31F9F5DDE686ED39260681F29301AA878068B_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		CancellationToken_t51142D9C6D7C02D314DA34A6A7988C528992FFED L_0 = __this->___cancellationToken_2;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* CallOptions_get_WriteOptions_m7F6A4E257CA22FD8E4D3FA0633DACABA5B95817E_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		WriteOptions_tABB8D5678E45F0698B1B14FC5A59F1C55FACF050* L_0 = __this->___writeOptions_3;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* CallOptions_get_PropagationToken_m6FAE4454FD0AC717C9DFA54255ACCDA9D834C522_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		ContextPropagationToken_t3D20D09B421D355689C95D8C5F1BA4B1C4D18751* L_0 = __this->___propagationToken_4;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* CallOptions_get_Credentials_m36616F8A973A7EC538A623897CC7A0A832BACF9F_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		CallCredentials_t937C403ED70D8E5BC712464E26D2CD3EF0144466* L_0 = __this->___credentials_5;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t CallOptions_get_Flags_mA4C453704C2AC0C8E04BE00C588ACE4E1F8CC969_inline (CallOptions_t2092F3EA4E07BACD81EF531A7952510B8058218E* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->___flags_6;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR String_t* EncodingExtensions_GetString_m5290CA29398B11EA0507EEEBE965E7E78137EEFF_inline (Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* ___0_encoding, intptr_t ___1_ptr, int32_t ___2_len, const RuntimeMethod* method) 
{
	static bool s_Il2CppMethodInitialized;
	if (!s_Il2CppMethodInitialized)
	{
		il2cpp_codegen_initialize_runtime_metadata((uintptr_t*)&_stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709);
		s_Il2CppMethodInitialized = true;
	}
	{
		int32_t L_0 = ___2_len;
		if (!L_0)
		{
			goto IL_0012;
		}
	}
	{
		Encoding_t65CDEF28CF20A7B8C92E85A4E808920C2465F095* L_1 = ___0_encoding;
		void* L_2;
		L_2 = IntPtr_ToPointer_m1A0612EED3A1C8B8850BE2943CFC42523064B4F6_inline((&___1_ptr), NULL);
		int32_t L_3 = ___2_len;
		String_t* L_4;
		L_4 = EncodingExtensions_GetString_mF5E63092112C64E8D48B96690474700EBAFFD672(L_1, (uint8_t*)L_2, L_3, NULL);
		return L_4;
	}

IL_0012:
	{
		return _stringLiteralDA39A3EE5E6B4B0D3255BFEF95601890AFD80709;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t String_get_Length_m42625D67623FA5CC7A44D47425CE86FB946542D2_inline (String_t* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->____stringLength_4;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t Status_get_StatusCode_m36A97B664FAF32C62CC2BF1CF7D58D8AF65DE386_inline (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = __this->___U3CStatusCodeU3Ek__BackingField_2;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR String_t* Status_get_Detail_m27B09DC9C3459A6DE0461139E6F7012EA86C49BB_inline (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) 
{
	{
		String_t* L_0 = __this->___U3CDetailU3Ek__BackingField_3;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Exception_t* Status_get_DebugException_m434DF404A6BDDD4910C19272ED5516F95500A359_inline (Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642* __this, const RuntimeMethod* method) 
{
	{
		Exception_t* L_0 = __this->___U3CDebugExceptionU3Ek__BackingField_4;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void* IntPtr_ToPointer_m1A0612EED3A1C8B8850BE2943CFC42523064B4F6_inline (intptr_t* __this, const RuntimeMethod* method) 
{
	{
		intptr_t L_0 = *__this;
		return (void*)(L_0);
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR RuntimeObject* Func_2_Invoke_mDBA25DA5DA5B7E056FB9B026AF041F1385FB58A9_gshared_inline (Func_2_tACBF5A1656250800CE861707354491F0611F6624* __this, RuntimeObject* ___0_arg, const RuntimeMethod* method) 
{
	typedef RuntimeObject* (*FunctionPointerType) (RuntimeObject*, RuntimeObject*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, ___0_arg, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 Func_1_Invoke_m349A2DF2DF3672A6D87AA4E5E86E8AF54B24A956_gshared_inline (Func_1_tA0B1A86E85CD786E8C6BDE5FDC0CCFDF44B3BCF0* __this, const RuntimeMethod* method) 
{
	typedef Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 (*FunctionPointerType) (RuntimeObject*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 Func_2_Invoke_m497B8CC89F1330EB7265E70F432347348B580F14_gshared_inline (Func_2_tCF8881C5D96DB8E3391FEB4557496E47CE2526B2* __this, RuntimeObject* ___0_arg, const RuntimeMethod* method) 
{
	typedef Status_t06BFA824ACD77ABBCF5193ACF5C25E9C4EDF6642 (*FunctionPointerType) (RuntimeObject*, RuntimeObject*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, ___0_arg, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR RuntimeObject* Func_1_Invoke_m1412272198DFA4066C83206E5B43353AF10A2EEE_gshared_inline (Func_1_tD5C081AE11746B200C711DD48DBEB00E3A9276D4* __this, const RuntimeMethod* method) 
{
	typedef RuntimeObject* (*FunctionPointerType) (RuntimeObject*, const RuntimeMethod*);
	return ((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void Action_1_Invoke_mF2422B2DD29F74CE66F791C3F68E288EC7C3DB9E_gshared_inline (Action_1_t6F9EB113EB3F16226AEF811A2744F4111C116C87* __this, RuntimeObject* ___0_obj, const RuntimeMethod* method) 
{
	typedef void (*FunctionPointerType) (RuntimeObject*, RuntimeObject*, const RuntimeMethod*);
	((FunctionPointerType)__this->___invoke_impl_1)((Il2CppObject*)__this->___method_code_6, ___0_obj, reinterpret_cast<RuntimeMethod*>(__this->___method_3));
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR RuntimeObject* Enumerable_Empty_TisRuntimeObject_mA90CDE158774C34A28C07CEEA9E9EA2A61618238_gshared_inline (const RuntimeMethod* method) 
{
	{
		il2cpp_codegen_runtime_class_init_inline(il2cpp_rgctx_data(method->rgctx_data, 0));
		ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* L_0 = ((EmptyEnumerable_1_t8C8873EF4F89FB0F86D91BA5B4D640E3A23AD28E_StaticFields*)il2cpp_codegen_static_fields_for(il2cpp_rgctx_data(method->rgctx_data, 0)))->___Instance_0;
		return (RuntimeObject*)L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR int32_t List_1_get_Count_m4407E4C389F22B8CEC282C15D56516658746C383_gshared_inline (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, const RuntimeMethod* method) 
{
	{
		int32_t L_0 = (int32_t)__this->____size_2;
		return L_0;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void List_1_Add_mEBCF994CC3814631017F46A387B1A192ED6C85C7_gshared_inline (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, RuntimeObject* ___0_item, const RuntimeMethod* method) 
{
	ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* V_0 = NULL;
	int32_t V_1 = 0;
	{
		int32_t L_0 = (int32_t)__this->____version_3;
		__this->____version_3 = ((int32_t)il2cpp_codegen_add(L_0, 1));
		ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* L_1 = (ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918*)__this->____items_1;
		V_0 = L_1;
		int32_t L_2 = (int32_t)__this->____size_2;
		V_1 = L_2;
		int32_t L_3 = V_1;
		ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* L_4 = V_0;
		NullCheck(L_4);
		if ((!(((uint32_t)L_3) < ((uint32_t)((int32_t)(((RuntimeArray*)L_4)->max_length))))))
		{
			goto IL_0034;
		}
	}
	{
		int32_t L_5 = V_1;
		__this->____size_2 = ((int32_t)il2cpp_codegen_add(L_5, 1));
		ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* L_6 = V_0;
		int32_t L_7 = V_1;
		RuntimeObject* L_8 = ___0_item;
		NullCheck(L_6);
		(L_6)->SetAt(static_cast<il2cpp_array_size_t>(L_7), (RuntimeObject*)L_8);
		return;
	}

IL_0034:
	{
		RuntimeObject* L_9 = ___0_item;
		((  void (*) (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D*, RuntimeObject*, const RuntimeMethod*))il2cpp_codegen_get_method_pointer(il2cpp_rgctx_method(method->klass->rgctx_data, 11)))(__this, L_9, il2cpp_rgctx_method(method->klass->rgctx_data, 11));
		return;
	}
}
IL2CPP_MANAGED_FORCE_INLINE IL2CPP_METHOD_ATTR void List_1_Clear_m16C1F2C61FED5955F10EB36BC1CB2DF34B128994_gshared_inline (List_1_tA239CB83DE5615F348BB0507E45F490F4F7C9A8D* __this, const RuntimeMethod* method) 
{
	int32_t V_0 = 0;
	{
		int32_t L_0 = (int32_t)__this->____version_3;
		__this->____version_3 = ((int32_t)il2cpp_codegen_add(L_0, 1));
		if (!true)
		{
			goto IL_0035;
		}
	}
	{
		int32_t L_1 = (int32_t)__this->____size_2;
		V_0 = L_1;
		__this->____size_2 = 0;
		int32_t L_2 = V_0;
		if ((((int32_t)L_2) <= ((int32_t)0)))
		{
			goto IL_003c;
		}
	}
	{
		ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918* L_3 = (ObjectU5BU5D_t8061030B0A12A55D5AD8652A20C922FE99450918*)__this->____items_1;
		int32_t L_4 = V_0;
		Array_Clear_m50BAA3751899858B097D3FF2ED31F284703FE5CB((RuntimeArray*)L_3, 0, L_4, NULL);
		return;
	}

IL_0035:
	{
		__this->____size_2 = 0;
	}

IL_003c:
	{
		return;
	}
}
