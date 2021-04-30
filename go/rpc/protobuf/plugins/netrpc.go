package plugins

import (
	"bytes"
	"log"
	"text/template"

	"github.com/golang/protobuf/protoc-gen-go/descriptor"
	"github.com/golang/protobuf/protoc-gen-go/generator"
)
const tmplService = `
{{$root := .}}

type {{.ServiceName}}Interface interface {
	{{- range $_, $m := .MethodList}}
	{{$m.MethodName}}(*{{$m.InputTypeName}}, *{{$m.OutputTypeName}}) error
	{{- end}}
}
func Register{{.ServiceName}}(srv *rpc.Server, x {{.ServiceName}}Interface) error {
	if err := srv.RegisterName("{{.ServiceName}}", x); err != nil {
		return err
	}
	return nil
}
type {{.ServiceName}}Client struct {
	*rpc.Client
}
var _ {{.ServiceName}}Interface = (*{{.ServiceName}}Client)(nil)
func Dial{{.ServiceName}}(network, address string) (*{{.ServiceName}}Client, error) {
	c, err := rpc.Dial(network, address)
	if err != nil {
		return nil, err
	}
	return &{{.ServiceName}}Client{Client: c}, nil
}
{{range $_, $m := .MethodList}}
func (p *{{$root.ServiceName}}Client) {{$m.MethodName}}(in *{{$m.InputTypeName}}, out *{{$m.OutputTypeName}}) error {
	return p.Client.Call("{{$root.ServiceName}}.{{$m.MethodName}}", in, out)
}
{{end}}

`
type netrpcPlugin struct{*generator.Generator}

func (np *netrpcPlugin)Name()string{
	return "netrpc"
}

func (np *netrpcPlugin)Init(g *generator.Generator){
	np.Generator = g
}

func(np *netrpcPlugin)GenerateImports(file *generator.FileDescriptor){
	if len(file.Service) > 0{
		np.genImportCode(file)
	}
}

func(np *netrpcPlugin)Generate(file *generator.FileDescriptor){
	for _, svc := range file.Service {
		np.genServiceCode(svc)
	}
}

func (np *netrpcPlugin)genImportCode(file *generator.FileDescriptor){
	np.P(`import "net/rpc"`)
}

type ServiceSpec struct {
	ServiceName string
	MethodList []ServiceMethodSpec
}

type ServiceMethodSpec struct {
	MethodName string
	InputTypeName string
	OutputTypeName string
}

func (np *netrpcPlugin)buildServiceSpec(svc *descriptor.ServiceDescriptorProto)*ServiceSpec{
	spec := &ServiceSpec{
		ServiceName: generator.CamelCase(svc.GetName()),
	}
	for _, m := range svc.Method{
		spec.MethodList = append(spec.MethodList, ServiceMethodSpec{
			MethodName: generator.CamelCase(m.GetName()),
			InputTypeName: np.TypeName(np.ObjectNamed(m.GetInputType())),
			OutputTypeName: np.TypeName(np.ObjectNamed(m.GetOutputType())),
		})
	}
	return spec
}

func (np *netrpcPlugin)genServiceCode(svc *descriptor.ServiceDescriptorProto){
	spec := np.buildServiceSpec(svc)
	var buf bytes.Buffer
	t := template.Must(template.New("").Parse(tmplService))
	err := t.Execute(&buf, spec)
	if err != nil {
		log.Fatal(err)
	}
	np.P(buf.String())
}

func init() {
	generator.RegisterPlugin(new(netrpcPlugin))
}