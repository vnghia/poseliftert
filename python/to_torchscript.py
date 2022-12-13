import argparse

import torch
from pose_lifter import PoseLifter

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="to_torchscript",
        description="Convert a PoseLifter model from mmpose to torchscript",
    )
    parser.add_argument("-w", "--weights_path")
    parser.add_argument("-o", "--output")

    args = parser.parse_args()

    pose_lifter = PoseLifter(
        tcn_in_channels=2 * 17,
        head_in_channels=1024,
        num_joints=17,
        stem_channels=1024,
        num_blocks=4,
        kernel_sizes=(1, 1, 1, 1, 1),
        dropout=0.25,
        use_stride_conv=True,
    )
    pose_lifter.init_mmpose(args.weights_path)
    pose_lifter = pose_lifter.eval()
    scripted_module = torch.jit.script(
        pose_lifter, example_inputs={pose_lifter: [(torch.rand(1, 34, 1),)]}
    )
    scripted_module.save(args.output)
