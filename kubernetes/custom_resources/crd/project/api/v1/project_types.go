/*


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package v1

import (
	corev1 "k8s.io/api/core/v1"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
)

// EDIT THIS FILE!  THIS IS SCAFFOLDING FOR YOU TO OWN!
// NOTE: json tags are required.  Any new fields you add must have json tags for the fields to be serialized.

//BuildSpec defines the desired state of BuildSpec
type BuildSpec struct {
	CreateImage bool `json:"createImage,omitempty"`
	DefaultBranch string `json:"defaultBranch,omitempty"`
	Version string `json:"version,omitempty"`
	Dockerfile string `json:"dockerfile,omitempty"`
	BuildImage string `json:"buildImage,omitempty"`
	BuildCommand []string `json:"buildCommand,omitempty"`
}

//Template defines the desired state of RunSpec
type Template struct {
	Labels []string `json:"labels,omitempty"`
	Spec corev1.PodSpec `json:"spec,omitempty"`
	Replicas int `json:"replicas,omitempty"`
	RevisionHistoryLimit int `json:"revisionHistoryLimit,omitempty"`
	Namespace string `json:"namespace,omitempty"`
	RunEnv string `json:"runEnv,omitempty"`
}

//RegistrySpec defines the desired state of RunSpec
type RegistrySpec struct {
	Server string `json:"createimage,omitempty"`
	Namespace string `json:"namespace,omitempty"`
	Secret string `json:"secret,omitempty"`
}

// ProjectSpec defines the desired state of Project
type ProjectSpec struct {
	// INSERT ADDITIONAL SPEC FIELDS - desired state of cluster
	// Important: Run "make" to regenerate code after modifying this file

	// Foo is an example field of Project. Edit Project_types.go to remove/update
	Git string `json:"git,omitempty"`
	BuildSpec BuildSpec `json:"buildSpec,omitempty"`
	RegistrySpec RegistrySpec `json:"registrySpec,omitempty"`
	Template Template	`json:"template,omitempty"`
}

// ProjectStatus defines the observed state of Project
type ProjectStatus struct {
	// INSERT ADDITIONAL STATUS FIELD - define observed state of cluster
	// Important: Run "make" to regenerate code after modifying this file
}

// +kubebuilder:object:root=true

// Project is the Schema for the projects API
type Project struct {
	metav1.TypeMeta   `json:",inline"`
	metav1.ObjectMeta `json:"metadata,omitempty"`

	Spec   ProjectSpec   `json:"spec,omitempty"`
	Status ProjectStatus `json:"status,omitempty"`
}

// +kubebuilder:object:root=true

// ProjectList contains a list of Project
type ProjectList struct {
	metav1.TypeMeta `json:",inline"`
	metav1.ListMeta `json:"metadata,omitempty"`
	Items           []Project `json:"items"`
}

func init() {
	SchemeBuilder.Register(&Project{}, &ProjectList{})
}
