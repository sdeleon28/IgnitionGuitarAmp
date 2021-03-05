import os
import sys
import shutil


def replace_version(line, version):
    return line.replace('[$VERSION]', version)

def main(version):
    script_path = os.path.dirname(os.path.realpath(__file__))
    template_path = os.path.join(script_path, 'windows-install-script.iss.template')
    installer_tmp_path = os.path.join(script_path, 'tmp')
    install_script_name =  'install-script.iss'
    tmp_installer_script_path = os.path.join(installer_tmp_path, install_script_name)
    project_dir = os.path.dirname(os.path.dirname(script_path))
    output_installer_path = os.path.join(installer_tmp_path, 'Output/IgnitionGuitarAmp-windows.exe')
    installer_copy_path = os.path.join(script_path, 'out/IgnitionGuitarAmp_windows_{}.exe'.format(version))
    vst3_path = os.path.join(project_dir, 'Builds/VisualStudio2019/x64/Release/VST3/IgnitionGuitarAmp.vst3')
    shutil.rmtree(installer_tmp_path, ignore_errors=True)
    try:
        os.mkdir(installer_tmp_path)
        shutil.copy(vst3_path, installer_tmp_path)
        template_lines = []
        with open(template_path, 'r') as template:
            template_lines = template.readlines()
        script_lines = [replace_version(line, version) for line in template_lines]
        with open(tmp_installer_script_path, 'w') as installer_script:
            installer_script.writelines(script_lines)
        cwd = os.getcwd()
        try:
            os.chdir(installer_tmp_path)
            os.system('iscc {}'.format(install_script_name))
        except Exception as error:
            print('error: {}'.format(error))
        finally:
            os.chdir(cwd)
        shutil.copy(output_installer_path, installer_copy_path)
    finally:
        shutil.rmtree(installer_tmp_path, ignore_errors=True)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: python3 makeinstaller.py VERSION')
    else:
        version = sys.argv[1]
        main(version)
